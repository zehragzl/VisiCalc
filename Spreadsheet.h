#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Cell.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>
#include <iostream>

namespace Spreadsheet {

    class OutOfBounds : public std::runtime_error {
    public:
        OutOfBounds(const std::string& message);
    };

    class InvalidFormula : public std::runtime_error {
    public:
        InvalidFormula(const std::string& message);
    };

    template <class T>
    class Grid {
    public:
        Grid(int rows, int cols);
        std::unique_ptr<Cell>& at(int row, int col);
        const std::unique_ptr<Cell>& at(int row, int col) const;

    private:
        int rows, cols;
        std::unique_ptr<std::unique_ptr<Cell>[]> grid;
    };

    class Spreadsheet {
    private:
        Grid<Cell> grid;
        int rows, cols;

        

    public:
        Spreadsheet(int rows, int cols);
        void setCell(int row, int col, std::unique_ptr<Cell> cell);
        Cell* getCell(int row, int col) const;
        void enterCellData(int row, int col, const std::string& input);

        template <typename T>
        T getCellContent(int row, int col) const {
            Cell* cell = getCell(row, col);
            if (!cell) {
                throw std::runtime_error("Cell does not exist or is empty.");
            }
            if constexpr (std::is_same_v<T, std::string>) {
                return cell->getDisplayValue();
            } else if constexpr (std::is_same_v<T, int>) {
                IntValueCell* intCell = dynamic_cast<IntValueCell*>(cell);
                if (intCell) return intCell->getValue();
            } else if constexpr (std::is_same_v<T, double>) {
                DoubleValueCell* doubleCell = dynamic_cast<DoubleValueCell*>(cell);
                if (doubleCell) return doubleCell->getValue();
            }
            throw std::runtime_error("Invalid type or cell does not exist.");
        }

		std::string getCellReference(int row, int col) const;
        void recalculate();
        void displaySpreadsheet(int maxRows, int maxCols) const;

        int getRowCount() const;
        int getColCount() const;

    };

}
#endif
