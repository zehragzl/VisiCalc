#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Cell.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
#include <type_traits>  // For std::is_same

namespace Spreadsheet {

    // Exception class for out-of-bounds errors in the spreadsheet
    class OutOfBounds : public std::runtime_error {
    public:
        OutOfBounds(const std::string& message);  // Constructor for custom error messages
    };

    // Exception class for invalid formula errors
    class InvalidFormula : public std::runtime_error {
    public:
        InvalidFormula(const std::string& message);  // Constructor for custom error messages
    };

    // Template class for a 2D grid to hold cells
    template <class T>
    class Grid {
    public:
        Grid(int rows, int cols);  // Constructor to initialize grid with specified rows and columns
        std::unique_ptr<Cell>& at(int row, int col);  // Accessor for a specific cell (non-const)
        const std::unique_ptr<Cell>& at(int row, int col) const;  // Accessor for a specific cell (const)

    private:
        int rows, cols;  // Number of rows and columns in the grid
        std::unique_ptr<std::unique_ptr<Cell>[]> grid;  // 2D array of unique pointers to cells
    };

    // Main class for the Spreadsheet
    class Spreadsheet {
    private:
        Grid<Cell> grid;  // The 2D grid that stores cells
        int rows, cols;  // Number of rows and columns in the spreadsheet

    public:
        Spreadsheet(int rows, int cols);  // Constructor to initialize the spreadsheet

        // Method to set a cell at a specific row and column
        void setCell(int row, int col, std::unique_ptr<Cell> cell);
        
        // Method to get a pointer to a cell at a specific row and column
        Cell* getCell(int row, int col) const;

        // Method to enter data into a cell
        void enterCellData(int row, int col, const std::string& input);

        // Template method to retrieve the content of a cell based on its type
        template <typename T>
        T getCellContent(int row, int col) const {
            Cell* cell = getCell(row, col);  // Get the cell at the given position
            if (!cell) {
                throw std::runtime_error("Cell does not exist or is empty.");  // If the cell is empty, throw an error
            }

            // Check the type and return the appropriate value
            if (std::is_same<T, std::string>::value) {
                return cell->getDisplayValue();  // Return the cell's string value
            } else if (std::is_same<T, int>::value) {
                IntValueCell* intCell = dynamic_cast<IntValueCell*>(cell);
                if (intCell) return intCell->getDisplayValue();  // Return integer value if the cell contains an integer
            } else if (std::is_same<T, double>::value) {
                DoubleValueCell* doubleCell = dynamic_cast<DoubleValueCell*>(cell);
                if (doubleCell) return doubleCell->getDisplayValue();  // Return double value if the cell contains a double
            }

            // If no valid type is found, throw an error
            throw std::runtime_error("Invalid type or cell does not exist.");
        }

        // Method to get the reference (e.g., A1, B2) of a specific cell
        std::string getCellReference(int row, int col) const;

        // Method to display the spreadsheet on the screen
        void displaySpreadsheet(int maxRows, int maxCols) const;

        // Getters for row and column count
        int getRowCount() const;
        int getColCount() const;
    };

}

#endif  // End of the Spreadsheet header guard
