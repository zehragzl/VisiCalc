#include "Spreadsheet.h"
#include "Cell.h"
#include "AnsiTerminal.h"
#include <iostream>
#include <memory>

namespace Spreadsheet {


template std::string Spreadsheet::getCellContent<std::string>(int, int) const;

    // Spreadsheet.cpp dosyasına ekleme yapın
OutOfBounds::OutOfBounds(const std::string& message) : std::runtime_error(message) {}

InvalidFormula::InvalidFormula(const std::string& message) : std::runtime_error(message) {}

template <class T>
Grid<T>::Grid(int rows, int cols) : rows(rows), cols(cols) {
    grid = std::make_unique<std::unique_ptr<Cell>[]>(rows * cols);
}

template <class T>
std::unique_ptr<Cell>& Grid<T>::at(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw OutOfBounds("Grid index out of bounds");
    }
    return grid[row * cols + col];
}

template <class T>
const std::unique_ptr<Cell>& Grid<T>::at(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw OutOfBounds("Grid index out of bounds");
    }
    return grid[row * cols + col];
}

Spreadsheet::Spreadsheet(int rows, int cols) : rows(rows), cols(cols), grid(rows, cols) {}

    int Spreadsheet::getRowCount() const { return rows; } // Satır sayısını döner
    int Spreadsheet::getColCount() const { return cols; } // Sütun sayısını döner

void Spreadsheet::setCell(int row, int col, std::unique_ptr<Cell> cell) {
    grid.at(row - 1, col - 1) = std::move(cell); // Adjusted for 1-based indexing
}

Cell* Spreadsheet::getCell(int row, int col) const {
    const auto& cellPtr = grid.at(row - 1, col - 1); // Adjusted for 1-based indexing
    return cellPtr ? cellPtr.get() : nullptr;
}

void Spreadsheet::enterCellData(int row, int col, const std::string& input) {
    if (row <= 0 || row > rows || col <= 0 || col > cols) {
        std::cerr << "Error: Invalid cell reference." << std::endl;
        return;
    }

    if (input[0] == '=') { // Formula handling
        Cell* cell = getCell(row, col);
        FormulaCell* formulaCell = dynamic_cast<FormulaCell*>(cell);
        if (formulaCell) {
            formulaCell->setFormula(input);
            formulaCell->setNeedsRecalculation(true);
        } else {
            setCell(row, col, std::make_unique<FormulaCell>(input));
        }
    } else { // Value handling
        try {
            double doubleValue = std::stod(input); // Try to parse as double
            if (doubleValue == static_cast<int>(doubleValue)) { 
                // Check if it is a whole number (integer)
                setCell(row, col, std::make_unique<IntValueCell>(static_cast<int>(doubleValue)));
            } else {
                setCell(row, col, std::make_unique<DoubleValueCell>(doubleValue));
            }
        } catch (const std::invalid_argument&) {
            setCell(row, col, std::make_unique<StringValueCell>(input)); // If not a number, treat as string
        }
    }

    displaySpreadsheet(rows, cols);
}




void Spreadsheet::recalculate() {
    // Recalculate all formulas based on dependencies.
}

void Spreadsheet::displaySpreadsheet(int maxRows, int maxCols) const {
    AnsiTerminal terminal;
    terminal.clearScreen();

    for (int col = 1; col <= maxCols && col <= cols; ++col) {
        char colLabel = 'A' + (col - 1);
        terminal.printAt(3, col * 10, std::string(1, colLabel));
    }

    for (int row = 1; row <= maxRows && row <= rows; ++row) {
        terminal.printAt(row + 3, 0, std::to_string(row));

        for (int col = 1; col <= maxCols && col <= cols; ++col) {
            std::string cellContent = "";
            Cell* cell = getCell(row, col);

            if (cell) {
                cellContent = cell->getDisplayValue();
            }

            if (cellContent.size() > 10) {
                cellContent = cellContent.substr(0, 10);
            }

            terminal.printAt(row + 3, col * 10, cellContent);
        }
    }
}

std::string Spreadsheet::getCellReference(int row, int col) const {
    std::string colRef;
    while (col > 0) {
        colRef = char('A' + (col - 1) % 26) + colRef;
        col = (col - 1) / 26;
    }
    return colRef + std::to_string(row);
}




} // namespace Spreadsheet
