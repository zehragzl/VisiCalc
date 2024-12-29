#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>
#include <vector>
#include <cmath>

namespace Spreadsheet {

class FormulaParser {
public:
    FormulaParser(int rows, int cols, std::unordered_map<std::string, double> cellValues);
    int columnToIndex(const std::string& col);
    std::string indexToCell(int row, int col);
    // Formula parsing methods
    double evaluateExpression(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
    double evaluateFormula(const std::string& formula, std::unordered_set<std::string>& processing);

private:
    // Helpers for formula operations
    double evaluateSum(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
     double evaluateAverage(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);
    double evaluateStdDev(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);
    double evaluateMax(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);
    double evaluateMin(const std::string& formula, const std::unordered_map<std::string, double>& cellValues);

    // Helper functions
    bool isValidCellReference(const std::string& ref);
    void extractCellRange(const std::string& range, int& startRow, int& startCol, int& endRow, int& endCol);

    int rows, cols;
    std::unordered_map<std::string, double> cellValues;
};

} // namespace Spreadsheet

#endif // FORMULAPARSER_H
