# Notes for the author

Practical advice for making the method your own — the mindset, the tool, and
the habits that keep prose and code moving together.


## You and the AI are co-authors

Writing in LDD is like writing a book: you and the AI write it together, as
co-authors. It is a continuous dialog, not a queue of tickets handed down. You
bring the domain, the judgement and the *why*; the AI brings speed, recall and
tireless verification.

Practically, that means arguing with it. Ask it which invariant you left
unstated, which concept owns what, which stage does not actually prove
anything. A co-author that only agrees is not helping.


## Literate programming is a different way to code

It is its own paradigm, not a documentation habit bolted onto a normal
workflow. Getting familiar with its basic ideas — chunks as a meta-language,
*web not tree*, presenting parts in the order that teaches rather than the
order the compiler wants — is worth the afternoon it takes, and it is what lets
you get the most out of LDD.


## This workflow is a proposal

It is deliberately simple, and it is not the definitive one. Adapt it to your
own style: there are as many ways to write as there are authors. What should
survive any adaptation is the Prime Directive — change the code, update the
prose — because that is the part everything else is protecting.


## Try pure LDD first

No spec-driven development, no harness engineering, no agentic loops, no
generated helper scripts. You may add any of them later; none are
indispensable, and adding them early hides how much the method itself was
doing. The whole toolchain is one command: `weft`.


## You understand the code from day one

At every moment the author knows and understands the code that was generated —
from the first line. That is the real difference from prompting a model for a
file and hoping. If you find yourself unable to explain a chunk you just
accepted, the chapter is not done: that is exactly the gap review exists to
find.


## One simple command: weft

LDD runs on `weft`, a single command that interprets the weft metalanguage —
binding one formatting language (LaTeX, HTML or Markdown) with one or more
programming languages at once. Everything else in the method is discipline,
not tooling.


## Two outputs, one source

`.weft` files are organized around mental concepts, in a metalanguage designed
to be at once **explained to humans** and **understood by computers**. *Weave*
them and you get a PDF that reads and renders as a book; *tangle* them and you
get the source code the compilers build — both from the same file, never out of
step.


## A concept crosses every layer and language

A single `.weft` concept can hold as many pieces of different languages as the
project needs. Login, for instance, is transversal to many systems and
subsystems: `login.weft` can gather fragments of Go, shell, SQL, TypeScript,
JavaScript, HTML, Terraform and Python, plus external calls in REST and
GraphQL — with their tests — all in one file: login, expressed conceptually.

Any improvement to login means reading a single file. It does not touch other
concepts, or only tangentially, so things do not break elsewhere. The
cross-language *contract* ends up explained in one place — exactly where
multi-language bugs hide.


## Software is evolutionary

Software evolves. LDD lets you build incrementally, to the needs of the day.
Everything is built on what came before — which is what makes it fast, cheap
and safe to ship. A stage boundary is an edition of the book: it tangles,
builds green, and reads complete for what exists.


## Change the code, update the prose

Any change to a code fragment must update its prose. The prose says the *why*
and the *what*; the code says the *how*. That pairing is what stays valuable —
today and in the future — for the programmers and the AIs who inherit the
project. Code alone tells them what happens; only the prose tells them what was
meant.


## Every pass can enrich the prose

Each review is also a chance to improve the exposition. With LaTeX you have
complex equations, tables, cross-references, diagrams and the full typographic
richness of publishing at your disposal — all in service of explaining the code
better. A chapter can keep getting better long after it stopped changing
behaviour.


---

> Every step forward, you understand the code, you watch your project evolve on
> solid ground, and you turn it into a work of art. No friction, fast, and with
> great token savings. **Enjoy.**
