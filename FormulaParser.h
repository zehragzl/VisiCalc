#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <vector>
#include <cmath>

namespace Spreadsheet {

    // Class to parse and evaluate formulas in a spreadsheet
    class FormulaParser {
    public:
        // Constructor initializes the spreadsheet dimensions and cell values
        FormulaParser(int rows, int cols, std::unordered_map<std::string, double> cellValues);

        // Converts column letter (e.g., "A") to column index
        int columnToIndex(const std::string& col);

        // Converts row and column index to cell reference (e.g., "A1")
        std::string indexToCell(int row, int col);

        // Evaluates an expression (e.g., "A1 + B1") based on cell values
        double evaluateExpression(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);

        // Evaluates a formula and handles circular references
        double evaluateFormula(const std::string& formula, std::unordered_set<std::string>& processing);

    private:
        // Evaluates SUM formula (e.g., "SUM(A1..A10)")
        double evaluateSum(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);

        // Evaluates AVERAGE formula (e.g., "AVERAGE(A1..A10)")
        double evaluateAverage(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);

        // Evaluates STDDEV formula (e.g., "STDDEV(A1..A10)")
        double evaluateStdDev(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);

        // Evaluates MAX formula (e.g., "MAX(A1..A10)")
        double evaluateMax(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);

        // Evaluates MIN formula (e.g., "MIN(A1..A10)")
        double evaluateMin(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);

        // Checks if a given cell reference is valid
        bool isValidCellReference(const std::string& ref);

        // Extracts the start and end row/column from a cell range (e.g., "A1..A10")
        void extractCellRange(const std::string& range, int& startRow, int& startCol, int& endRow, int& endCol);

        int rows, cols;  // Dimensions of the spreadsheet
        std::unordered_map<std::string, double> cellValues;  // Cell values in the spreadsheet
    };

} // namespace Spreadsheet

#endif // FORMULAPARSER_H
