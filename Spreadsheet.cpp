#include "Spreadsheet.h"
using namespace std;

namespace Spreadsheet{

    template <class T>
    Grid<T>::Grid(int rows, int cols) : rows(rows), cols(cols) {
        grid = make_unique<unique_ptr<T[]>[]>(rows);
        for(int i=0; i<rows; ++i){
            grid[i] = make_unique<T[]>(cols);
        }
    }


    template <class T>
    T& Grid<T>::at(int row, int col) {
        if(row<0 || row>=rows || col<0 || col>=cols) {
            throw OutOfBounds("Cell reference out of bounds!");
        }
        return grid[row][col];
    }

    template <class T>
    const T& Grid<T>::at(int row, int col) const {
        if(row<0 || row>=rows || col<0 || col>=cols) {
            throw OutOfBounds("Cell reference out of bounds!");
        }
        return grid[row][col];
    }

    Spreadsheet::Spreadsheet(int rows, int cols) : grid(rows,cols), rows(rows), cols(cols) {}

    Cell* Spreadsheet::getCell(int row, int col) const {
        return &grid.at(row,col);
    }

    // Getter methods for rows and columns count
    int Spreadsheet::getCols() const {
        return cols;
    }

    int Spreadsheet::getRows() const {
        return rows;
    }

    //Validation method to check if a given cell (row, col) is within bounds
    bool Spreadsheet::isValidCell(int row, int col) const {
        return row >= 1 && row <= rows && col >= 1 && col <= cols;
    }

    // Method to input data into a specific cell
    void Spreadsheet::enterCellData(int row, int col, const std::string& input) {
        if (!isValidCell(row, col)) {
            throw OutOfBounds("Cell reference out of bounds!");
        }

        int adjustedRow = row - 1; // Convert to 0-based
        int adjustedCol = col - 1;

        // Check if the input starts with '=' (indicating a formula)
        if (input[0] == '=') {
            // Store the formula without evaluating it
            cells[adjustedRow][adjustedCol].setFormula(input); // Set formula
            cells[adjustedRow][adjustedCol].setNeedsRecalculation(true); // Mark as needing recalculation
        } else {
            // Check if the input is numeric (either integer or floating point)
            try {
                double numericValue = std::stod(input); // Try converting to number
                cells[adjustedRow][adjustedCol].setValue(numericValue);
                cells[adjustedRow][adjustedCol].setNeedsRecalculation(false); // No recalculation needed
            } catch (const std::invalid_argument&) {
                // If it's not a valid number, treat it as a string
                cells[adjustedRow][adjustedCol].setValue(input);
                cells[adjustedRow][adjustedCol].setNeedsRecalculation(false); // No recalculation needed
            }
        }
        recalculate();   
        }



    // Setter methods for setting content in a specific cell
    void Spreadsheet::setCellContent(int row, int col, const std::string& content) {
        if (!isValidCell(row, col)) {
            throw OutOfBounds("Cell reference out of bounds!");
        }
        getCell(row, col)->setContent(content);
    }

    // Method to retrieve the reference (e.g., A1, B2) of a specific cell	
    string Spreadsheet::getCellReference(int row, int col) const {
        if (!isValidCell(row, col)) {
            throw OutOfBounds("Cell reference out of bounds!");
        }
        return getCell(row, col)->getReference();
    }

    // Method to display the spreadsheet with a specific max number of rows and columns
    void Spreadsheet::displaySpreadsheet(int maxRows, int maxCols) const {
        for (int r = 0; r < std::min(rows, maxRows); ++r) {
            for (int c = 0; c < std::min(cols, maxCols); ++c) {
                std::cout << getCellContent<std::string>(r, c) << "\t";
            }
            std::cout << std::endl;
        }
    }


    void Spreadsheet::recalculate() {
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                try {
                    std::string formula = getCellContent<std::string>(r, c);
                    if (!formula.empty()) {
                        getCell(r, c)->setValue(formulaParser.evaluateFormula(formula, r, c));
                    }
                } catch (const InvalidFormulaException& e) {
                    std::cerr << "Error in formula at " << r << "," << c << ": " << e.what() << std::endl;
                }
            }
        }
    }
}




/*
// Helper method to evaluate a formula for a specific cell
double Spreadsheet::evaluateFormula(Cell &cell, const std::string &currentCell, std::unordered_set<std::string> &processing) {
    if (processing.find(currentCell) != processing.end()) {
        throw std::runtime_error("Cyclic dependency detected in cell: " + currentCell);
    }

    processing.insert(currentCell);

    std::string formula = cell.getFormula();
    if (!formula.empty() && formula[0] == '=') {
        formula = formula.substr(1);
    }

    std::unordered_map<std::string, double> cellValues;
    for (int row = 1; row <= rows; ++row) {
        for (int col = 1; col <= cols; ++col) {
            const std::string cellRef = getCellReference(row, col);
            const Cell &c = cells[row - 1][col - 1];
            if (c.getType() == Cell::ValueType::NUMBER) {
                cellValues[cellRef] = c.getNumber();
            } else if (c.getType() == Cell::ValueType::EMPTY) {
                cellValues[cellRef] = 0.0;
            }
        }
    }

    try {
        double result = 0.0;

        if (formula.find("SUM") == 0) {
            result = evaluateSum(formula, cellValues);
        } else if (formula.find("AVER") == 0) {
            result = evaluateAverage(formula, cellValues);
        } else if (formula.find("STDDEV") == 0) {
            result = evaluateStdDev(formula, cellValues);
        } else if (formula.find("MAX") == 0) {
            result = evaluateMax(formula, cellValues);
        } else if (formula.find("MIN") == 0) {
            result = evaluateMin(formula, cellValues);
        } else {
            // General arithmetic expression evaluation
            result = formulaParser.evaluateExpression(formula, cellValues);
        }

        cell.setNumber(result);
        processing.erase(currentCell);

        // Print the evaluated result
        std::cout << "Cell " << currentCell << " evaluated to: " << result << std::endl;

        return result;
    } catch (const std::exception &e) {
        std::cerr << "Error evaluating formula for cell " << currentCell << ": " << e.what() << std::endl;
        processing.erase(currentCell);
        throw;
    }
}
// Methods to evaluate various functions (SUM, AVERAGE, STDDEV, MAX, MIN) in a formula
double Spreadsheet::evaluateSum(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    // Regex pattern to match SUM expressions, e.g., SUM(A1..A4)
    std::regex rangeRegex("SUM\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    // Check if the expression matches the SUM pattern
    if (std::regex_search(expr, match, rangeRegex)) {
        std::string startCell = match[1];  // e.g., "A1"
        std::string endCell = match[2];    // e.g., "A4"

        // Parse the starting cell reference (e.g., "A1")
        int startRow = std::stoi(startCell.substr(1)) - 1;  // Convert "1" in "A1" to 0-indexed row
        int startCol = startCell[0] - 'A';                 // Convert "A" to 0-indexed column

        // Parse the ending cell reference (e.g., "A4")
        int endRow = std::stoi(endCell.substr(1)) - 1;
        int endCol = endCell[0] - 'A';

        // Ensure the range is valid
        if (startRow > endRow || startCol > endCol) {
            throw std::runtime_error("Invalid range in SUM expression: " + expr);
        }

        // Sum the values in the specified range
        double sum = 0.0;
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = getCellReference(r + 1, c + 1);  // Convert to 1-based reference
                if (cellValues.find(cellRef) != cellValues.end()) {
                    sum += cellValues.at(cellRef);  // Add the cell value if it exists
                } else {
                    std::cerr << "Warning: Cell " << cellRef << " is empty or non-numeric.\n";
                }
            }
        }
        return sum;
    }

    // If the expression doesn't match the SUM pattern, throw an error
    throw std::runtime_error("Invalid SUM expression: " + expr);
}
double Spreadsheet::evaluateAverage(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("AVER\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        std::string startCell = match[1];
        std::string endCell = match[2];

        int startRow = std::stoi(startCell.substr(1)) - 1;
        int startCol = startCell[0] - 'A';

        int endRow = std::stoi(endCell.substr(1)) - 1;
        int endCol = endCell[0] - 'A';

        if (startRow > endRow || startCol > endCol) {
            throw std::runtime_error("Invalid range in AVERAGE expression: " + expr);
        }

        double sum = 0.0;
        int count = 0;

        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = getCellReference(r + 1, c + 1);
                auto it = cellValues.find(cellRef);
                if (it != cellValues.end()) {
                    sum += it->second;
                    count++;
                }
            }
        }

        if (count > 0) {
            return sum / count;
        } else {
            return 0.0;
        }
    }

    throw std::runtime_error("Invalid AVERAGE expression: " + expr);
}
double Spreadsheet::evaluateStdDev(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("STDDEV\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        std::string startCell = match[1];
        std::string endCell = match[2];

        int startRow = std::stoi(startCell.substr(1)) - 1;
        int startCol = startCell[0] - 'A';

        int endRow = std::stoi(endCell.substr(1)) - 1;
        int endCol = endCell[0] - 'A';

        if (startRow > endRow || startCol > endCol) {
            throw std::runtime_error("Invalid range in STDDEV expression: " + expr);
        }

        std::vector<double> values;

        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = getCellReference(r + 1, c + 1);
                auto it = cellValues.find(cellRef);
                if (it != cellValues.end()) {
                    values.push_back(it->second);
                }
            }
        }

        if (values.empty()) {
            throw std::runtime_error("No valid values found for STDDEV calculation");
        }

        double mean = std::accumulate(values.begin(), values.end(), 0.0) / values.size();
        double sumSquaredDiffs = 0.0;
        for (double value : values) {
            sumSquaredDiffs += std::pow(value - mean, 2);
        }

        return std::sqrt(sumSquaredDiffs / values.size());
    }

    throw std::runtime_error("Invalid STDDEV expression: " + expr);
}
double Spreadsheet::evaluateMax(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("MAX\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        std::string startCell = match[1];
        std::string endCell = match[2];

        int startRow = std::stoi(startCell.substr(1)) - 1;
        int startCol = startCell[0] - 'A';

        int endRow = std::stoi(endCell.substr(1)) - 1;
        int endCol = endCell[0] - 'A';

        if (startRow > endRow || startCol > endCol) {
            throw std::runtime_error("Invalid range in MAX expression: " + expr);
        }

        double maxValue = std::numeric_limits<double>::lowest();
        bool hasValue = false;

        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = getCellReference(r + 1, c + 1);
                auto it = cellValues.find(cellRef);
                if (it != cellValues.end()) {
                    maxValue = std::max(maxValue, it->second);
                    hasValue = true;
                }
            }
        }

        if (!hasValue) {
            throw std::runtime_error("No valid values found for MAX calculation");
        }

        return maxValue;
    }

    throw std::runtime_error("Invalid MAX expression: " + expr);
}
double Spreadsheet::evaluateMin(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("MIN\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        std::string startCell = match[1];
        std::string endCell = match[2];

        int startRow = std::stoi(startCell.substr(1)) - 1;
        int startCol = startCell[0] - 'A';

        int endRow = std::stoi(endCell.substr(1)) - 1;
        int endCol = endCell[0] - 'A';

        if (startRow > endRow || startCol > endCol) {
            throw std::runtime_error("Invalid range in MIN expression: " + expr);
        }

        double minValue = std::numeric_limits<double>::max();
        bool hasValue = false;

        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = getCellReference(r + 1, c + 1);
                auto it = cellValues.find(cellRef);
                if (it != cellValues.end()) {
                    minValue = std::min(minValue, it->second);
                    hasValue = true;
                }
            }
        }

        if (!hasValue) {
            throw std::runtime_error("No valid values found for MIN calculation");
        }

        return minValue;
    }

    throw std::runtime_error("Invalid MIN expression: " + expr);
}

*/