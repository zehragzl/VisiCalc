<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17"/>
  <img src="https://img.shields.io/badge/build-make%20%7C%20cmake-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="Build"/>
  <img src="https://img.shields.io/badge/platform-linux%20%7C%20macOS-lightgrey?style=for-the-badge&logo=linux&logoColor=white" alt="Platform"/>
  <img src="https://img.shields.io/badge/license-MIT-green?style=for-the-badge" alt="License"/>
</p>

<h1 align="center">📊 VisiCalc</h1>
<h3 align="center">A Modern Terminal-Based Spreadsheet Engine</h3>

<p align="center">
  <i>Feature-rich spreadsheet application built from scratch in C++17 — formula parsing, cell referencing, statistical functions, CSV persistence, and an interactive ANSI terminal interface.</i>
</p>

---

## 🎯 Overview

**VisiCalc** is a terminal-based spreadsheet application that reimagines the classic spreadsheet concept as a command-line tool. Built entirely in C++17, it features a **recursive descent formula parser**, **polymorphic cell type system**, and **real-time ANSI terminal rendering** — all with zero external dependencies.

> This project was designed to demonstrate proficiency in advanced C++ programming, object-oriented architecture, and algorithm design.

---

## ✨ Features

| Category | Details |
|:---------|:--------|
| **Interactive UI** | Navigate cells with arrow keys, real-time ANSI terminal rendering |
| **Type System** | Polymorphic cells: `int`, `double`, `string`, `formula` |
| **Formula Engine** | Recursive descent parser with cell reference resolution |
| **Statistical Functions** | `SUM`, `AVER`, `STDDEV`, `MAX`, `MIN` over cell ranges |
| **Circular Reference Detection** | Prevents infinite loops in formula evaluation |
| **File I/O** | CSV import/export with automatic type inference |
| **Dynamic Grid** | Configurable display dimensions (up to 100×100) |

---

## 🏗️ Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                        main.cpp                                  │
│                   Application Entry Point                        │
│              Input Loop · Display Management                     │
├──────────────────┬──────────────────┬───────────────────────────┤
│   AnsiTerminal   │   Spreadsheet    │      FileManager          │
│  ─────────────   │  ─────────────   │    ─────────────          │
│  Terminal I/O    │  Grid<T> Mgmt    │    CSV Save/Load          │
│  Cursor Control  │  Cell Operations │    Type Inference          │
│  Key Handling    │  Display Render  │                            │
├──────────────────┴────────┬─────────┴───────────────────────────┤
│                     FormulaParser                                │
│               ──────────────────────                             │
│       Recursive Descent · Cell Range Resolution                  │
│          SUM · AVER · STDDEV · MAX · MIN                         │
├──────────────────────────────────────────────────────────────────┤
│                    Cell Hierarchy                                 │
│                                                                   │
│                    Cell (abstract)                                 │
│                    ├── FormulaCell                                 │
│                    └── ValueCell (abstract)                        │
│                        ├── IntValueCell                            │
│                        ├── DoubleValueCell                         │
│                        └── StringValueCell                         │
└──────────────────────────────────────────────────────────────────┘
```

### Design Patterns & Principles

- **Polymorphism** — Abstract `Cell` base class with virtual dispatch for type-safe cell operations
- **Template Programming** — Generic `Grid<T>` for compile-time type safety
- **RAII** — `std::unique_ptr` throughout; `AnsiTerminal` restores terminal state on destruction
- **Strategy Pattern** — `FormulaParser` evaluates different formula types through unified interface
- **Separation of Concerns** — Distinct modules for UI, data, parsing, and persistence

---

## 🚀 Getting Started

### Prerequisites

| Tool | Version | Required |
|:-----|:--------|:---------|
| **g++** / **clang++** | C++17 support | ✅ |
| **GNU Make** | 3.81+ | ✅ (or use CMake) |
| **CMake** | 3.16+ | Optional |
| **clang-format** | Any | Optional |
| **cppcheck** | Any | Optional |

### Build & Run

#### Option 1 — Make (Recommended)

```bash
# Clone
git clone https://github.com/zehragzl/VisiCalc.git
cd VisiCalc

# Build (release)
make

# Build (debug with AddressSanitizer)
make debug

# Run
make run

# See all available targets
make help
```

#### Option 2 — CMake

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./VisiCalc
```

#### Option 3 — Manual

```bash
g++ -std=c++17 -O2 -o visicalc main.cpp Cell.cpp Spreadsheet.cpp \
    FormulaParser.cpp FileManager.cpp AnsiTerminal.cpp
./visicalc
```

---

## 📖 Usage

### Keyboard Controls

| Key | Action |
|:----|:-------|
| `↑ ↓ ← →` | Navigate between cells |
| `E` | Enter/Edit cell data |
| `G` | Resize display grid |
| `s` | Save spreadsheet to CSV |
| `l` | Load spreadsheet from CSV |
| `q` | Quit application |

### Entering Data

1. Navigate to the target cell with arrow keys
2. Press **`E`** to enter edit mode
3. Type your data:
   - **Numbers:** `42`, `3.14`
   - **Text:** `Hello World`
   - **Formulas:** `=A1+B2`, `=SUM(A1..A10)`
4. Press **Enter** to confirm

### Formula Examples

```
=A1+B1              # Basic arithmetic
=A1*2+B1/3          # Mixed operations
=SUM(A1..A10)       # Sum of range
=AVER(B1..B5)       # Average of range
=STDDEV(C1..C20)    # Standard deviation
=MAX(A1..D1)        # Maximum in range
=MIN(A1..A100)      # Minimum in range
```

---

## 🛠️ Technical Highlights

### Smart Memory Management
```cpp
// Polymorphic cell storage with zero manual memory management
std::unique_ptr<std::unique_ptr<Cell>[]> grid;

// Type-safe cell creation
setCell(row, col, std::make_unique<DoubleValueCell>(3.14));
```

### Recursive Formula Evaluation
```cpp
// Circular reference detection using processing set
double evaluateFormula(const std::string& formula,
                       std::unordered_set<std::string>& processing);
```

### Template-Based Grid
```cpp
// Generic grid with bounds checking and const-correct accessors
template <class T>
class Grid {
    std::unique_ptr<Cell>& at(int row, int col);
    const std::unique_ptr<Cell>& at(int row, int col) const;
};
```

---

## 📁 Project Structure

```
VisiCalc/
├── main.cpp              # Entry point — input loop & display management
├── Cell.h / Cell.cpp     # Polymorphic cell hierarchy (4 concrete types)
├── Spreadsheet.h / .cpp  # Grid management, data entry, display rendering
├── FormulaParser.h / .cpp# Recursive descent parser & statistical functions
├── FileManager.h / .cpp  # CSV serialization / deserialization
├── AnsiTerminal.h / .cpp # POSIX terminal control (raw mode, cursor, keys)
├── Makefile              # GNU Make build system (debug/release/install)
├── CMakeLists.txt        # CMake build configuration
├── CONTRIBUTING.md       # Contribution guidelines
├── LICENSE               # MIT License
└── README.md             # This file
```

---

## 🔮 Roadmap

- [ ] Undo/Redo functionality with command pattern
- [ ] Copy/Paste operations across cell ranges
- [ ] Cell formatting (colors, alignment, borders)
- [ ] Additional functions: `MEDIAN`, `MODE`, `COUNT`, `IF`
- [ ] Multi-sheet support with cross-sheet references
- [ ] Export to Excel (`.xlsx`) format
- [ ] ncurses-based UI for richer rendering

---

## 🤝 Contributing

Contributions are welcome! Please read the [Contributing Guide](CONTRIBUTING.md) before submitting a PR.

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'feat: add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

Distributed under the **MIT License**. See [LICENSE](LICENSE) for details.

---

<p align="center">
  Built with ❤️ as an academic project demonstrating advanced C++ programming
</p>
