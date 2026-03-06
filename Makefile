CC     ?= cc
CFLAGS ?= -g -O2 -std=c11 -Wall -Wextra -pedantic

SRCDIR  = src
BUILDDIR = build

SRCS = main.c pass1.c latex.c markdown.c output.c input.c scraps.c names.c \
       arena.c global.c
OBJS = $(patsubst %.c,$(BUILDDIR)/%.o,$(SRCS))
HDRS = $(SRCDIR)/global.h

TEXENV = TEXINPUTS=bib:.: BIBINPUTS=bib: BSTINPUTS=bib:

# ─── Main targets ───────────────────────────────────────────────────

.PHONY: all bootstrap weft check doc user-guide dist clean veryclean

all: weft

# First build from a fresh clone (uses pre-generated .c in src/).
# After this, ./weft exists and you can run 'make weft'.
bootstrap: $(OBJS)
	$(CC) -o weft $(OBJS)

# Normal development: tangle .weft sources into src/, then compile.
weft:
	./weft -t -p $(SRCDIR)/ weft.weft
	@$(MAKE) --no-print-directory bootstrap

# ─── Compile rules ─────────────────────────────────────────────────

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(HDRS) | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# ─── Tests ──────────────────────────────────────────────────────────

check:
	@n=0; f=0; \
	for i in test/*/*.sh; do \
	  echo "Testing $$i"; \
	  sh "$$i"; \
	  if [ $$? -ne 0 ]; then \
	    echo "         $$i FAILED"; \
	    f=$$((f + 1)); \
	  fi; \
	  n=$$((n + 1)); \
	done; \
	echo "$$n tests, $$f failed"; \
	[ $$f -eq 0 ]

# ─── Documentation ─────────────────────────────────────────────────

# Developer reference (full source with code listings)
doc:
	@if [ -z "$(VERSION)" ]; then \
	  echo "Error: VERSION is required.  Usage: make doc VERSION=1.0.2"; \
	  exit 1; \
	fi
	./weft -o -w tex -r -V "$(VERSION)" weft.weft
	-$(TEXENV) pdflatex -interaction=nonstopmode weft.tex
	-$(TEXENV) bibtex weft
	./weft -o -w tex -r -V "$(VERSION)" weft.weft
	-$(TEXENV) pdflatex -interaction=nonstopmode weft.tex
	-$(TEXENV) pdflatex -interaction=nonstopmode weft.tex
	@rm -f weft.tex weft.aux weft.log weft.out weft.toc weft.brf \
	       weft.bbl weft.blg
	@echo "→ weft.pdf"

# User guide (introduction chapter only)
user-guide:
	@if [ -z "$(VERSION)" ]; then \
	  echo "Error: VERSION is required.  Usage: make user-guide VERSION=1.0.2"; \
	  exit 1; \
	fi
	./weft -o -w tex -r -V "$(VERSION)" weft-user-guide.weft
	-$(TEXENV) pdflatex -interaction=nonstopmode weft-user-guide.tex
	-$(TEXENV) bibtex weft-user-guide
	-$(TEXENV) pdflatex -interaction=nonstopmode weft-user-guide.tex
	-$(TEXENV) pdflatex -interaction=nonstopmode weft-user-guide.tex
	@rm -f weft-user-guide.tex weft-user-guide.aux weft-user-guide.log \
	       weft-user-guide.out weft-user-guide.toc weft-user-guide.brf \
	       weft-user-guide.bbl weft-user-guide.blg
	@echo "→ weft-user-guide.pdf"

# ─── Distribution ──────────────────────────────────────────────────

# Prepare the repo for release: tangle, compile, test, generate docs.
# If ./weft doesn't exist yet, bootstrap from pre-generated src/*.c first.
dist:
	@if [ -z "$(VERSION)" ]; then \
	  echo "Error: VERSION is required.  Usage: make dist VERSION=1.0.2"; \
	  exit 1; \
	fi
	@if [ ! -x ./weft ]; then \
	  echo "No weft binary found, bootstrapping from src/..."; \
	  $(MAKE) --no-print-directory bootstrap; \
	fi
	$(MAKE) --no-print-directory weft
	$(MAKE) --no-print-directory clean
	$(MAKE) --no-print-directory check
	$(MAKE) --no-print-directory doc VERSION=$(VERSION)
	$(MAKE) --no-print-directory user-guide VERSION=$(VERSION)
	@echo ""
	@echo "Distribution ready (version $(VERSION)). Generated files:"
	@echo "  weft                  (binary)"
	@echo "  src/*.c src/*.h       (bootstrap sources)"
	@echo "  weft.pdf              (developer reference)"
	@echo "  weft-user-guide.pdf   (user guide)"

# ─── Clean ──────────────────────────────────────────────────────────

# Remove build artifacts (object files).
clean:
	rm -rf $(BUILDDIR)
	-rm -f *~ *.lint

# Remove everything generated (binary, docs, LaTeX intermediates).
veryclean: clean
	-rm -f weft weft.pdf weft-user-guide.pdf
	-rm -f weft.tex weft.aux weft.log weft.out weft.toc weft.brf
	-rm -f weft.bbl weft.blg
	-rm -f weft-user-guide.tex weft-user-guide.aux weft-user-guide.log
	-rm -f weft-user-guide.out weft-user-guide.toc weft-user-guide.brf
	-rm -f weft-user-guide.bbl weft-user-guide.blg
	@echo "src/*.c preserved (bootstrap). Delete manually if needed."
