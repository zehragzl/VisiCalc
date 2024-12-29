#include "Spreadsheet.h"
#include "Cell.h"
#include "AnsiTerminal.h"
#include "FormulaParser.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace Spreadsheet {

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

int Spreadsheet::getRowCount() const { return rows; }

int Spreadsheet::getColCount() const { return cols; }

void Spreadsheet::setCell(int row, int col, std::unique_ptr<Cell> cell) {
    grid.at(row - 1, col - 1) = std::move(cell); // Adjusted for 1-based indexing
}

Cell* Spreadsheet::getCell(int row, int col) const {
    const auto& cellPtr = grid.at(row - 1, col - 1); // Adjusted for 1-based indexing
    return cellPtr ? cellPtr.get() : nullptr;
}

void Spreadsheet::enterCellData(int row, int col, const std::string& input) {
    if (row <= 0 || row > rows || col <= 0 || col > cols) {
        std::cerr << "Hata: Geçersiz hücre referansı." << std::endl;
        return;
    }

    // Geçici olarak cellValues haritasını oluşturun
    std::unordered_map<std::string, double> cellValues;  // Now it's a map of double

    // Mevcut hücre verilerini haritaya ekleyin
    for (int r = 1; r <= rows; ++r) {
        for (int c = 1; c <= cols; ++c) {
            std::string cellRef = getCellReference(r, c);
            Cell* cell = getCell(r, c);
            
            // If the cell is a ValueCell, we extract the value (assumed to be a double)
            if (cell) {
                try {
                    cellValues[cellRef] = cell->value();  // Store the value of the cell
                } catch (const std::exception&) {
                    cellValues[cellRef] = 0.0;  // If value() throws, we assume it's 0.0 for formulas
                }
            }
        }
    }

    if (input[0] == '=') { // Formül işleme
        FormulaParser parser(rows, cols, cellValues);  // Pass the new map of doubles

        std::unordered_set<std::string> processing;
        try {
            std::string formula = input.substr(1); // '=' karakterini kaldır
            double result = parser.evaluateFormula(formula, processing);

            // Sonucu hücreye kaydet
            setCell(row, col, std::make_unique<DoubleValueCell>(result));
        } catch (const std::exception& e) {
            std::cerr << "Formülde hata: " << e.what() << std::endl;
        }
    } else { // Değer işleme
        try {
            double doubleValue = std::stod(input);
            if (doubleValue == static_cast<int>(doubleValue)) {
                setCell(row, col, std::make_unique<IntValueCell>(static_cast<int>(doubleValue)));
            } else {
                setCell(row, col, std::make_unique<DoubleValueCell>(doubleValue));
            }
        } catch (const std::invalid_argument&) {
            setCell(row, col, std::make_unique<StringValueCell>(input));
        }
    }

    displaySpreadsheet(rows, cols); // Güncellenen tabloyu görüntüle
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
                cellContent = cellContent.substr(0, 10); // Limit content length
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
