#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <string>
#include <stdexcept>
#include "Spreadsheet.h"

namespace Spreadsheet {

class FormulaParser {
public:
    // Constructor
    FormulaParser(const Spreadsheet& spreadsheet);

    // Parse a formula (e.g., "=A1+A2")
    double parseFormula(const std::string& formula);

private:
    const Spreadsheet& spreadsheet;

    // Trim leading and trailing spaces
    std::string trim(const std::string& str) const;

    // Parse a cell reference (e.g., "A1" -> row and column indices)
    bool parseCellReference(const std::string& ref, int& row, int& col) const;
};

} // namespace Spreadsheet

#endif // FORMULAPARSER_H
