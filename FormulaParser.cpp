#include "FormulaParser.h"
#include <sstream>
#include <iostream>

namespace Spreadsheet {

// Constructor to initialize the spreadsheet
FormulaParser::FormulaParser(const Spreadsheet& spreadsheet) : spreadsheet(spreadsheet) {}

// Parse a formula like "=A1+A2"
double FormulaParser::parseFormula(const std::string& formula) {
    std::string formulaBody = formula.substr(1); // Remove leading '='

    // Split the formula by the '+' sign (for simplicity, only considering addition)
    std::stringstream ss(formulaBody);
    std::string token;
    double result = 0;

    while (std::getline(ss, token, '+')) {
        // Trim spaces
        token = trim(token);

        // Extract cell reference (e.g., "A1")
        int row = 0, col = 0;
        if (parseCellReference(token, row, col)) {
            // Fetch the cell's value
            Cell* cell = spreadsheet.getCell(row, col);
            if (!cell) {
                throw std::runtime_error("Cell " + token + " does not exist.");
            }

            // Add the cell's value to the result (assuming numerical cells)
            result += cell->value();
        } else {
            throw std::invalid_argument("Invalid cell reference: " + token);
        }
    }
    return result;
}

// Trim leading and trailing spaces
std::string FormulaParser::trim(const std::string& str) const {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Parse a cell reference like "A1" to extract row and column indices
bool FormulaParser::parseCellReference(const std::string& ref, int& row, int& col) const {
    if (ref.empty()) return false;

    // Extract column (e.g., "A" -> 1)
    col = ref[0] - 'A' + 1;

    // Extract row (e.g., "1" -> 1)
    try {
        row = std::stoi(ref.substr(1));
    } catch (const std::invalid_argument&) {
        return false;
    }

    return true;
}

} // namespace Spreadsheet
