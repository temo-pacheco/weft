# LDD bootstrap: a double-entry ledger

The reference project for **Literate Driven Development**. One source
(`.weft`) produces both a working C program and a typeset book. The only tool
is `weft`.

## Layout

```
project.weft                 master: LaTeX + @i includes (weaves the book)
literate/
├── 00-domain.weft           the domain and the problem (prose only)
├── 01-architecture.weft     concepts, decision records, dependency contract
├── 02-roadmap.weft          the stages + the state of the work
├── concepts/
│   ├── accounts.weft        the store and its balance operations
│   ├── ledger.weft          transactions + the double-entry invariant
│   └── api.weft             the walking-skeleton main
└── assembly.weft            the ONLY file with @o (routes chunks → src/*)
```

## Pipeline

```bash
export WEFT=../../weft                      # or wherever your weft binary is

mkdir -p src                                # weft does not create parent dirs
weft project.weft                           # tangle → src/*.c, src/*.h
cc -Wall src/*.c -o ledger && ./ledger      # build + run the skeleton

weft -w tex project.weft && pdflatex project.tex   # weave the book
```

## The review, before a chapter closes

```bash
weft --lint project.weft          # invented chunk names, orphan chunks
weft --diff project.weft          # empty = the tangled src/ is current
cc -Wall src/*.c -o ledger 2>&1 | weft --errors    # errors → .weft lines
#  + re-read the prose of every chunk touched   (the Prime Directive)
#  + read the chapter straight through          (does it read as a book?)
```

No runner, no hook, no lock file: five checks, three of them one `weft`
command each, two of them reading.

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
