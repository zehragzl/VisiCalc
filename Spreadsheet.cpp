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

    // Constructor for OutOfBounds exception, initializing with a message
    OutOfBounds::OutOfBounds(const std::string& message) : std::runtime_error(message) {}

    // Constructor for InvalidFormula exception, initializing with a message
    InvalidFormula::InvalidFormula(const std::string& message) : std::runtime_error(message) {}

    // Constructor for Grid class, initializing grid with the specified rows and columns
   template <class T>
Grid<T>::Grid(int rows, int cols) : rows(rows), cols(cols) {
    grid = std::make_unique<std::unique_ptr<Cell>[]>(rows * cols);
}

template <class T>
std::unique_ptr<Cell>& Grid<T>::at(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw OutOfBounds("Accessing out-of-bounds cell.");
    }
    return grid[row * cols + col];
}

template <class T>
const std::unique_ptr<Cell>& Grid<T>::at(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw OutOfBounds("Accessing out-of-bounds cell.");
    }
    return grid[row * cols + col];
}

    // Constructor for the Spreadsheet class
    Spreadsheet::Spreadsheet(int rows, int cols) : rows(rows), cols(cols), grid(rows, cols) {}

    // Getter for row count
    int Spreadsheet::getRowCount() const { return rows; }

    // Getter for column count
    int Spreadsheet::getColCount() const { return cols; }

    // Function to set a cell's value by creating a unique pointer to the appropriate cell type
    void Spreadsheet::setCell(int row, int col, std::unique_ptr<Cell> cell) {
    grid.at(row - 1, col - 1) = std::move(cell);
}

Cell* Spreadsheet::getCell(int row, int col) const {
    const auto& cellPtr = grid.at(row - 1, col - 1);
    return cellPtr ? cellPtr.get() : nullptr;
}

    // Function to enter data into a cell, handles both formulas and values
    void Spreadsheet::enterCellData(int row, int col, const std::string& input) {
        // Check if the provided row and col are valid
        if (row <= 0 || row > rows || col <= 0 || col > cols) {
            std::cerr << "Hata: Geçersiz hücre referansı." << std::endl; // Error message for invalid reference
            return;
        }

        // Temporary map to hold existing cell values for formula evaluation
        std::unordered_map<std::string, double> cellValues;  // Map holds double values for formulas

        // Add existing cell data to the map
        for (int r = 1; r <= rows; ++r) {
            for (int c = 1; c <= cols; ++c) {
                std::string cellRef = getCellReference(r, c);  // Generate cell reference (e.g., A1, B2)
                Cell* cell = getCell(r, c);
                
                // If the cell exists, store its value in the map
                if (cell) {
                    try {
                        cellValues[cellRef] = cell->value();  // Store the value of the cell
                    } catch (const std::exception&) {
                        cellValues[cellRef] = 0.0;  // Default to 0.0 for formulas or cells without a value
                    }
                }
            }
        }

        // If the input starts with '=', it's a formula, otherwise it's a regular value
        if (input[0] == '=') {  // Formula processing
            FormulaParser parser(rows, cols, cellValues);  // Pass the map of existing cell values
            std::unordered_set<std::string> processing;  // Set to keep track of cells being processed

            try {
                std::string formula = input.substr(1); // Remove '=' character
                double result = parser.evaluateFormula(formula, processing);  // Evaluate the formula

                // Save the result of the formula in the cell
                setCell(row, col, std::unique_ptr<DoubleValueCell>(new DoubleValueCell(result)));
            } catch (const std::exception& e) {
                std::cerr << "Formülde hata: " << e.what() << std::endl;  // Handle formula errors
            }
        } else {  // Value processing
            try {
                double doubleValue = std::stod(input);  // Attempt to convert the input to a double
                // If the double value is actually an integer, store it as an integer
                if (doubleValue == static_cast<int>(doubleValue)) {
                    setCell(row, col, std::unique_ptr<IntValueCell>(new IntValueCell(static_cast<int>(doubleValue))));
                } else {
                    setCell(row, col, std::unique_ptr<DoubleValueCell>(new DoubleValueCell(doubleValue)));
                }
            } catch (const std::invalid_argument&) {  // If conversion fails, store as a string
                setCell(row, col, std::unique_ptr<StringValueCell>(new StringValueCell(input)));
            }
        }

        // Display the updated spreadsheet after entering the data
        displaySpreadsheet(rows, cols);
    }

    // Function to display the spreadsheet in a terminal window, formatted with AnsiTerminal
    void Spreadsheet::displaySpreadsheet(int maxRows, int maxCols) const {
        AnsiTerminal terminal;
        terminal.clearScreen();  // Clear the screen before displaying

        // Print column headers (A, B, C, etc.)
        for (int col = 1; col <= maxCols && col <= cols; ++col) {
            char colLabel = 'A' + (col - 1);
            terminal.printAt(3, col * 10, std::string(1, colLabel));  // Column label position
        }

        // Print row numbers and cell values
        for (int row = 1; row <= maxRows && row <= rows; ++row) {
            terminal.printAt(row + 3, 0, std::to_string(row));  // Row number

            for (int col = 1; col <= maxCols && col <= cols; ++col) {
                std::string cellContent = "";
                Cell* cell = getCell(row, col);

                if (cell) {
                    cellContent = cell->getDisplayValue();  // Get the cell's display value
                }

                // Limit content length to fit the terminal display
                if (cellContent.size() > 10) {
                    cellContent = cellContent.substr(0, 10);  // Truncate content if necessary
                }

                terminal.printAt(row + 3, col * 10, cellContent);  // Display the content in the terminal
            }
        }
    }

    // Function to get the reference (e.g., A1, B2) of a specific cell based on its row and column
    std::string Spreadsheet::getCellReference(int row, int col) const {
        std::string colRef;
        while (col > 0) {
            colRef = char('A' + (col - 1) % 26) + colRef;  // Convert column number to letter
            col = (col - 1) / 26;  // Adjust column number for larger indexes
        }
        return colRef + std::to_string(row);  // Combine column letter and row number to form cell reference
    }

} // namespace Spreadsheet
