# ╔══════════════════════════════════════════════════════════════════════════════╗
# ║  VisiCalc — Terminal-Based Spreadsheet Application                         ║
# ║  Makefile                                                                  ║
# ╚══════════════════════════════════════════════════════════════════════════════╝

# ── Compiler Configuration ──────────────────────────────────────────────────────
CXX       := g++
CXXSTD    := -std=c++17
WARNINGS  := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wnon-virtual-dtor
OPTFLAGS  := -O2
DBGFLAGS  := -g -O0 -DDEBUG -fsanitize=address,undefined
LDFLAGS   :=
LDLIBS    :=

# ── Project Layout ──────────────────────────────────────────────────────────────
TARGET    := visicalc
SRCDIR    := .
BUILDDIR  := build
SOURCES   := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS   := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))
DEPENDS   := $(OBJECTS:.o=.d)

# ── Build Mode (default: release) ──────────────────────────────────────────────
#   make              → release build
#   make DEBUG=1      → debug build with sanitizers
ifdef DEBUG
  CXXFLAGS := $(CXXSTD) $(WARNINGS) $(DBGFLAGS)
  LDFLAGS  += -fsanitize=address,undefined
else
  CXXFLAGS := $(CXXSTD) $(WARNINGS) $(OPTFLAGS)
endif

# ── Prefix for install target ──────────────────────────────────────────────────
PREFIX    ?= /usr/local

# ══════════════════════════════════════════════════════════════════════════════════
#  Targets
# ══════════════════════════════════════════════════════════════════════════════════

.DEFAULT_GOAL := all

all: $(TARGET)   ## Build the project (release mode)

# Link
$(TARGET): $(OBJECTS)
	@echo "  LINK  $@"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	@echo ""
	@echo "  ✓ Build successful — run with: ./$(TARGET)"
	@echo ""

# Compile (with automatic dependency generation)
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	@echo "  CXX   $<"
	@$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILDDIR):
	@mkdir -p $(BUILDDIR)

# Include auto-generated dependency files
-include $(DEPENDS)

# ── Convenience Targets ────────────────────────────────────────────────────────

.PHONY: run debug clean distclean rebuild install uninstall format lint loc help

run: $(TARGET)   ## Build and run the application
	@./$(TARGET)

debug:   ## Build in debug mode (with AddressSanitizer)
	@$(MAKE) DEBUG=1

clean:   ## Remove build artifacts
	@echo "  CLEAN"
	@rm -rf $(BUILDDIR) $(TARGET)

distclean: clean   ## Remove all generated files including backups
	@rm -f *.csv *.bak *~

rebuild: clean all   ## Full rebuild from scratch

install: $(TARGET)   ## Install to $(PREFIX)/bin
	@echo "  INSTALL  $(PREFIX)/bin/$(TARGET)"
	@install -d $(PREFIX)/bin
	@install -m 755 $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:   ## Remove from $(PREFIX)/bin
	@echo "  UNINSTALL  $(PREFIX)/bin/$(TARGET)"
	@rm -f $(PREFIX)/bin/$(TARGET)

# ── Code Quality ───────────────────────────────────────────────────────────────

format:   ## Format source files with clang-format
	@echo "  FORMAT"
	@clang-format -i $(SRCDIR)/*.cpp $(SRCDIR)/*.h 2>/dev/null || \
		echo "  ⚠  clang-format not found — install it for auto-formatting"

lint:   ## Run static analysis with cppcheck
	@echo "  LINT"
	@cppcheck --enable=all --std=c++17 --suppress=missingInclude \
		--quiet $(SRCDIR)/*.cpp $(SRCDIR)/*.h 2>/dev/null || \
		echo "  ⚠  cppcheck not found — install it for static analysis"

# ── Information ────────────────────────────────────────────────────────────────

loc:   ## Count lines of code
	@echo ""
	@echo "  ┌─────────────────────────────────────┐"
	@echo "  │  Lines of Code                       │"
	@echo "  ├─────────────────────────────────────┤"
	@printf "  │  Headers (.h):  %6d              │\n" $$(cat $(SRCDIR)/*.h 2>/dev/null | wc -l)
	@printf "  │  Sources (.cpp): %5d              │\n" $$(cat $(SRCDIR)/*.cpp 2>/dev/null | wc -l)
	@printf "  │  Total:         %6d              │\n" $$(cat $(SRCDIR)/*.h $(SRCDIR)/*.cpp 2>/dev/null | wc -l)
	@echo "  └─────────────────────────────────────┘"
	@echo ""

help:   ## Show this help message
	@echo ""
	@echo "  VisiCalc — Build System"
	@echo "  ═══════════════════════════════════════"
	@echo ""
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | \
		awk 'BEGIN {FS = ":.*?## "}; {printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2}'
	@echo ""
	@echo "  Options:"
	@echo "    DEBUG=1       Enable debug mode with sanitizers"
	@echo "    PREFIX=path   Set install prefix (default: /usr/local)"
	@echo ""
