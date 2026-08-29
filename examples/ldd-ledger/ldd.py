#!/usr/bin/env python3
"""ldd — Literate Driven Development tooling, in one file, stdlib only.

A single cross-platform CLI that replaces the bash gate scripts and the task
runner. Its only dependency is `python3` and the `weft` binary. No jq, no
shasum, no openssl, no bash, no just.

    ldd verify [--build CMD]       run the five gates in order
    ldd conformance                gate 3: declared graph vs weft -m
    ldd prose check                gate 5: Prime Directive (mechanical)
    ldd prose lock [--affirm]      record the reviewed prose/code state
    ldd context CONCEPT            orient: directed context pack for a concept

Global options:
    --main FILE   master .weft (default: $LDD_MAIN or project.weft)
    --weft PATH   weft binary   (default: $WEFT or weft on PATH)

The gates, cheap-to-expensive:
    1 Refs         weft --lint clean            (invented names, orphans)
    2 Sync         weft --diff clean            (stale tangled output)
    3 Conformance  declared graph vs weft -m    (forbidden concept edges)
    4 Build        build cmd green              (mapped to .weft via --errors)
    5 Prose        prose-lock clean             (code changed, prose stale)
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
from pathlib import Path

LOCK = ".ldd-prose-lock"
AFFIRM_LOG = ".ldd-prose-affirmations"
ARCH_ALLOW = re.compile(r"%\s*ARCH-ALLOW:\s*(.+)")


# ── weft plumbing ─────────────────────────────────────────────────────────

def weft(args, weft_bin, stdin=None):
    """Run the weft binary, returning (returncode, stdout, stderr)."""
    proc = subprocess.run(
        [weft_bin, *args], input=stdin,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True,
    )
    return proc.returncode, proc.stdout, proc.stderr


def weft_map(main, weft_bin, bodies=False, prose=False):
    """Return the parsed `weft -m` JSON map (optionally with bodies/prose)."""
    args = ["-m"]
    if bodies:
        args.append("--bodies")
    if prose:
        args.append("--prose")
    args.append(main)
    rc, out, err = weft(args, weft_bin)
    if not out.strip():
        die(f"weft -m produced no output for {main}\n{err}")
    return json.loads(out)


def sha(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()


def concept_of(name: str):
    """The concept prefix of a chunk name: text before the first ':'.
    Returns None for names with no ':' (they carry no concept)."""
    if ":" not in name:
        return None
    return name.split(":", 1)[0].strip()


def die(msg: str, code: int = 2):
    print(msg, file=sys.stderr)
    sys.exit(code)


def ok(msg: str):
    print(f"✓ {msg}")


def bad(msg: str):
    print(f"✗ {msg}")


# ── Gate 1: Refs ──────────────────────────────────────────────────────────

def gate_refs(main, weft_bin) -> bool:
    print("── Gate 1/5 · Refs (weft --lint) ──")
    rc, out, err = weft(["--lint", main], weft_bin)
    blob = (out + err)
    print(blob.rstrip())
    if re.search(r"undefined ref|unused frag", blob, re.IGNORECASE):
        bad("refs: undefined references or orphan chunks (see above)")
        return False
    ok("refs clean — every @<name@> resolves, no orphans")
    return True


# ── Gate 2: Sync ──────────────────────────────────────────────────────────

def gate_sync(main, weft_bin) -> bool:
    print("── Gate 2/5 · Sync (weft --diff) ──")
    rc, out, err = weft(["--diff", main], weft_bin)
    if out.strip():
        print(out.rstrip())
        bad("tangled output is stale — re-tangle and commit")
        return False
    ok("output in sync — re-tangling would change nothing")
    return True


# ── Gate 3: Conformance ───────────────────────────────────────────────────

def allowed_edges(search_dir: Path):
    """All `% ARCH-ALLOW: A -> B` edges declared under search_dir."""
    edges = set()
    for path in search_dir.rglob("*.weft"):
        try:
            for line in path.read_text(encoding="utf-8", errors="replace").splitlines():
                m = ARCH_ALLOW.search(line)
                if m:
                    edges.add(normalize_edge(m.group(1)))
        except OSError:
            continue
    return edges


def normalize_edge(text: str) -> str:
    return re.sub(r"\s+", " ", text).strip()


def real_edges(main, weft_bin):
    """Cross-concept edges from the code graph: using-concept -> used-concept.

    Built from each fragment's `used_by` plus a scrap->concept map from `defs`
    (the `uses` field is unreliable in some builds; `used_by` is not)."""
    m = weft_map(main, weft_bin)
    owner = {}  # scrap number -> concept
    for frag in m["fragments"]:
        c = concept_of(frag["name"])
        if c is None:
            continue
        for d in frag.get("defs", []):
            owner[d["scrap"]] = c
    edges = set()
    for frag in m["fragments"]:
        to = concept_of(frag["name"])
        if to is None:
            continue
        for ub in frag.get("used_by", []):
            frm = owner.get(ub["scrap"])
            if frm and to and frm != to:
                edges.add(f"{frm} -> {to}")
    return edges


def gate_conformance(main, weft_bin, arch_dir=None) -> bool:
    print("── Gate 3/5 · Conformance (declared graph vs weft -m) ──")
    search = Path(arch_dir) if arch_dir else Path(main).resolve().parent
    allow = allowed_edges(search)
    if not allow:
        print(f"ℹ no '% ARCH-ALLOW:' policy under {search}. Nothing to")
        print("  enforce yet (Phase 0 has not declared the graph).")
        return True
    real = real_edges(main, weft_bin)
    violations = sorted(e for e in real if e not in allow)
    if violations:
        for e in violations:
            print(f"    ✗ forbidden edge: {e}")
        bad(f"{len(violations)} architecture violation(s). Fix the code, or")
        print("  add the '% ARCH-ALLOW:' and say why in the architecture chapter.")
        return False
    ok("every cross-concept edge is permitted by the architecture")
    return True


# ── Gate 4: Build ─────────────────────────────────────────────────────────

def gate_build(main, weft_bin, build_cmd) -> bool:
    print(f"── Gate 4/5 · Build ({build_cmd}) ──")
    proc = subprocess.run(build_cmd, shell=True,
                          stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    # Annotate compiler errors with .weft source locations.
    rc, annotated, _ = weft(["--errors"], weft_bin, stdin=proc.stdout)
    print((annotated or proc.stdout).rstrip())
    if proc.returncode != 0:
        bad("build/test failed — errors annotated with .weft source above")
        return False
    ok("build green")
    return True


# ── Gate 5: Prose (the Prime Directive) ───────────────────────────────────

def current_manifest(main, weft_bin):
    """name -> {code, prose, code_blank, prose_blank} for every fragment."""
    m = weft_map(main, weft_bin, bodies=True, prose=True)
    manifest = {}
    for frag in m["fragments"]:
        code = " ".join(d.get("body", "") for d in frag.get("defs", []))
        prose = " ".join(d.get("prose", "") for d in frag.get("defs", []))
        manifest[frag["name"]] = {
            "code": sha(code),
            "prose": sha(prose),
            "code_blank": code.strip() == "",
            "prose_blank": prose.strip() == "",
        }
    return manifest


def load_lock():
    p = Path(LOCK)
    if not p.exists():
        return {}
    try:
        return json.loads(p.read_text(encoding="utf-8"))
    except (OSError, json.JSONDecodeError):
        return {}


def prose_violations(manifest, lock):
    """List of (name, reason) for the failures a machine can detect."""
    out = []
    for name, cur in manifest.items():
        prev = lock.get(name)
        if prev:
            if cur["code"] != prev["code"] and cur["prose"] == prev["prose"]:
                out.append((name, "code changed, prose unchanged"))
        else:
            if not cur["code_blank"] and cur["prose_blank"]:
                out.append((name, "new code chunk has no prose"))
    return out


def gate_prose(main, weft_bin) -> bool:
    print("── Gate 5/5 · Prose (Prime Directive — prose-lock) ──")
    violations = prose_violations(current_manifest(main, weft_bin), load_lock())
    if violations:
        bad("PRIME DIRECTIVE VIOLATION — prose is stale:")
        for name, why in violations:
            print(f"    • {name}  ({why})")
        print("\n  Update the prose around these chunks so it describes the new")
        print("  code. If it is genuinely still correct, record it:")
        print(f"    ldd prose lock --affirm")
        return False
    ok("prose in sync — no chunk changed code while leaving prose stale")
    print("  Reviewer still confirms SEMANTIC coherence (does the prose tell the truth?):")
    print(f"    weft -m --prose --bodies {main}")
    return True


def cmd_prose(args):
    weft_bin = args.weft
    if args.action == "check":
        sys.exit(0 if gate_prose(args.main, weft_bin) else 1)
    elif args.action == "lock":
        manifest = current_manifest(args.main, weft_bin)
        violations = prose_violations(manifest, load_lock())
        if violations and not args.affirm:
            bad("refusing to update the lock — these chunks changed code "
                "but not prose:")
            for name, _ in violations:
                print(f"    • {name}")
            print("\n  Review the prose. Change it, or affirm it explicitly:")
            print("    ldd prose lock --affirm")
            sys.exit(1)
        if violations and args.affirm:
            with open(AFFIRM_LOG, "a", encoding="utf-8") as fh:
                for name, _ in violations:
                    fh.write(f"AFFIRMED prose-still-correct: {name}\n")
            print(f"ℹ recorded {len(violations)} affirmation(s) → {AFFIRM_LOG}")
        lock = {n: {"code": v["code"], "prose": v["prose"]}
                for n, v in manifest.items()}
        Path(LOCK).write_text(
            json.dumps(lock, indent=2, sort_keys=True, ensure_ascii=False) + "\n",
            encoding="utf-8")
        ok(f"lock updated → {LOCK}")


# ── context (orient) ──────────────────────────────────────────────────────

def cmd_context(args):
    m = weft_map(args.main, args.weft, bodies=True, prose=True)
    hits = [f for f in m["fragments"]
            if f["name"].startswith(args.concept)]
    if not hits:
        print(f"no fragments whose name starts with {args.concept!r}")
        return
    print(f"── {len(hits)} fragment(s) for '{args.concept}' ──")
    for f in hits:
        d0 = (f.get("defs") or [{}])[0]
        n_used = len(f.get("used_by", []))
        print(f"\n▸ {f['name']}   (used {n_used}×)")
        prose = (d0.get("prose") or "").strip()
        if prose:
            print(f"  prose: {prose}")
        body = (d0.get("body") or "").strip()
        if body:
            first = body.splitlines()[0][:78]
            print(f"  code:  {first}")


# ── verify (orchestrate all five) ─────────────────────────────────────────

def cmd_verify(args):
    main, weft_bin = args.main, args.weft
    if not gate_refs(main, weft_bin):
        sys.exit(1)
    if not gate_sync(main, weft_bin):
        sys.exit(1)
    if not gate_conformance(main, weft_bin, args.arch_dir):
        sys.exit(1)
    if args.build:
        if not gate_build(main, weft_bin, args.build):
            sys.exit(1)
    else:
        print("── Gate 4/5 · Build (skipped: pass --build \"cmd\") ──")
    if not gate_prose(main, weft_bin):
        sys.exit(1)
    print("\nAll gates passed. After a legitimate no-prose-change, record it:")
    print("  ldd prose lock --affirm   # then commit the lock")


def cmd_conformance(args):
    sys.exit(0 if gate_conformance(args.main, args.weft, args.arch_dir) else 1)


# ── argument parsing ──────────────────────────────────────────────────────

def build_parser():
    p = argparse.ArgumentParser(prog="ldd", description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--main", default=os.environ.get("LDD_MAIN", "project.weft"),
                   help="master .weft (default: $LDD_MAIN or project.weft)")
    p.add_argument("--weft", default=os.environ.get("WEFT", "weft"),
                   help="weft binary (default: $WEFT or weft on PATH)")
    sub = p.add_subparsers(dest="cmd", required=True)

    v = sub.add_parser("verify", help="run the five gates in order")
    v.add_argument("--build", default=None, help="build command for gate 4")
    v.add_argument("--arch-dir", default=None, help="dir to scan for ARCH-ALLOW")
    v.set_defaults(func=cmd_verify)

    c = sub.add_parser("conformance", help="gate 3 standalone")
    c.add_argument("--arch-dir", default=None)
    c.set_defaults(func=cmd_conformance)

    pr = sub.add_parser("prose", help="Prime Directive gate + lock")
    pr.add_argument("action", choices=["check", "lock"])
    pr.add_argument("--affirm", action="store_true",
                    help="with lock: record that prose was reviewed and holds")
    pr.set_defaults(func=cmd_prose)

    ct = sub.add_parser("context", help="orient: context pack for a concept")
    ct.add_argument("concept")
    ct.set_defaults(func=cmd_context)
    return p


def main(argv=None):
    args = build_parser().parse_args(argv)
    args.func(args)


if __name__ == "__main__":
    main()
