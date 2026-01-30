# VisiCalc - Terminal-Based Spreadsheet Application

A feature-rich spreadsheet application built in C++ with formula parsing, file I/O capabilities, and an interactive terminal interface. This project demonstrates advanced object-oriented programming concepts, template-based design patterns, and real-time formula evaluation.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## ✨ Features

### Core Functionality
- **Interactive Terminal UI**: Navigate through cells using arrow keys with ANSI terminal support
- **Multiple Data Types**: Support for integers, doubles, strings, and formulas
- **Formula Evaluation**: Real-time calculation with support for cell references and mathematical operations
- **File Operations**: Save and load spreadsheets in CSV format
- **Dynamic Grid**: Configurable display dimensions (default 100x100 cells)

### Formula Support
- **Basic Arithmetic**: Addition, subtraction, multiplication, division
- **Cell References**: Reference other cells in formulas (e.g., `=A1+B2`)
- **Statistical Functions**:
  - `SUM(A1..A10)` - Calculate sum of a range
  - `AVERAGE(A1..A10)` - Calculate average
  - `STDDEV(A1..A10)` - Calculate standard deviation
  - `MAX(A1..A10)` - Find maximum value
  - `MIN(A1..A10)` - Find minimum value
- **Circular Reference Detection**: Prevents infinite loops in formula calculations

## 🏗️ Architecture

### Object-Oriented Design
The project follows SOLID principles with a clean class hierarchy:

```
Cell (Abstract Base Class)
├── FormulaCell - Handles formula parsing and evaluation
└── ValueCell (Abstract)
    ├── IntValueCell - Integer values
    ├── DoubleValueCell - Floating-point values
    └── StringValueCell - Text values
```

### Key Components
- **Spreadsheet**: Main grid management with template-based design
- **FormulaParser**: Recursive descent parser for formula evaluation
- **FileManager**: CSV import/export functionality
- **AnsiTerminal**: Cross-platform terminal control and input handling

## 🚀 Getting Started

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- Make (optional, for using Makefile)

### Building the Project

#### Using Makefile (Recommended)
```bash
# Compile the project
make

# Run the application
make run

# Clean build artifacts
make clean
```

#### Manual Compilation
```bash
# Linux/macOS
g++ -std=c++17 -o visicalc main.cpp Cell.cpp Spreadsheet.cpp FormulaParser.cpp FileManager.cpp AnsiTerminal.cpp

# Windows (MinGW)
g++ -std=c++17 -o visicalc.exe main.cpp Cell.cpp Spreadsheet.cpp FormulaParser.cpp FileManager.cpp AnsiTerminal.cpp
```

### Running the Application
```bash
# Linux/macOS
./visicalc

# Windows
visicalc.exe
```

## 📖 Usage Guide

### Keyboard Controls
| Key | Action |
|-----|--------|
| `Arrow Keys` | Navigate between cells (U/D/L/R) |
| `E` | Enter/Edit cell data |
| `G` | Adjust display grid dimensions |
| `s` | Save spreadsheet to CSV file |
| `l` | Load spreadsheet from CSV file |
| `q` | Quit application |

### Entering Data
1. Navigate to desired cell using arrow keys
2. Press `E` to enter edit mode
3. Type your data:
   - Numbers: `42` or `3.14`
   - Text: `Hello World`
   - Formulas: `=A1+B2` or `=SUM(A1..A10)`
4. Press `Enter` to confirm

### Example Formulas
```
=A1+B1          # Add two cells
=A1*2           # Multiply cell by constant
=SUM(A1..A10)   # Sum a range
=AVERAGE(B1..B5) # Calculate average
=MAX(C1..C20)   # Find maximum value
```

## 🛠️ Technical Highlights

- **Smart Pointers**: Extensive use of `std::unique_ptr` for automatic memory management
- **Template Programming**: Generic `Grid<T>` class for type-safe cell storage
- **Exception Handling**: Custom exception classes for error management
- **STL Containers**: Efficient use of `unordered_map` and `unordered_set`
- **RAII Principles**: Resource management through object lifetime

## 📁 Project Structure

```
VisiCalc/
├── main.cpp              # Application entry point and UI loop
├── Cell.h/cpp            # Cell class hierarchy
├── Spreadsheet.h/cpp     # Grid management and cell operations
├── FormulaParser.h/cpp   # Formula parsing and evaluation
├── FileManager.h/cpp     # CSV file I/O operations
├── AnsiTerminal.h/cpp    # Terminal control utilities
├── Makefile              # Build automation
└── README.md             # This file
```

## 🔮 Future Enhancements

- [ ] Undo/Redo functionality
- [ ] Copy/Paste operations
- [ ] Cell formatting (colors, alignment)
- [ ] More statistical functions (MEDIAN, MODE, etc.)
- [ ] Chart generation from data
- [ ] Multi-sheet support
- [ ] Export to Excel format

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

Built as an academic project to demonstrate:
- Advanced C++ programming techniques
- Object-oriented design patterns
- Data structure implementation
- Formula parsing and evaluation algorithms

---

**Note**: This is a terminal-based application. For the best experience, use a terminal with ANSI escape sequence support.
