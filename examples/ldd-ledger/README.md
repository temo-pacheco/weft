# LDD bootstrap: a double-entry ledger

The reference project for **Literate Driven Development**. One source
(`.weft`) produces both a working C program and a typeset book. Built with
`weft`, gated by `ldd`.

## Layout

```
project.weft                 master: LaTeX + @i includes (weaves the book)
literate/
├── 00-domain.weft           FRAME — the domain and the problem (prose only)
├── 01-architecture.weft     DECOMPOSE — concepts, ADRs, % ARCH-ALLOW policy
├── 02-roadmap.weft          SEQUENCE — stages + the concept ledger
├── concepts/
│   ├── accounts.weft        the store and its balance operations
│   ├── ledger.weft          transactions + the double-entry invariant
│   └── api.weft             the walking-skeleton main
└── assembly.weft            the ONLY file with @o (routes chunks → src/*)
ldd.py                       the five-gate CLI (Python stdlib)
pre-commit                   git hook enforcing the Prime Directive
```

## Pipeline

```bash
export WEFT=../../weft                      # or wherever your weft binary is

mkdir -p src                                # weft does not create parent dirs
weft project.weft                           # tangle → src/*.c, src/*.h
cc -Wall src/*.c -o ledger && ./ledger      # build + run the skeleton

python3 ldd.py prose lock                   # baseline the Prime Directive
python3 ldd.py verify --build "cc -Wall src/*.c -o ledger"   # the five gates

weft -w tex project.weft && pdflatex project.tex   # weave the book
```

## What the skeleton proves

```
posted transaction: OK
  cash       100.00
  revenue   -100.00
unbalanced transaction: REJECTED (correct)
sum of all balances: 0 cents (must be 0)
```

The invariant is enforced at write time: a transaction whose postings do not
sum to zero is refused, and the sum of all balances stays zero.

## Note on the book weave

The book weaves and compiles with a plain `weft -w tex` + `pdflatex`; no
post-processing is needed. (Historical: an earlier weft regression glued the
first code line to the `lstlisting` options, so a scrap beginning with `#`
broke `listings` with *"Illegal parameter number in `\lst@insertargs`"*. It was
fixed at the source, in `literate/latex-output.weft`, by emitting a newline
after the options' `]` and before `\end{lstlisting}`.)

Never edit `src/` or `project.tex` as source — only `.weft` files.
