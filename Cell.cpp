#include "Cell.h"

// FormulaCell: Represents a cell containing a formula
FormulaCell::FormulaCell(const string& formula) : formulaValue(formula) {}

// Sets a new formula for the cell
void FormulaCell::setFormula(const string& formula) {
    formulaValue = formula;
}
// Returns the formula stored in the cell
string FormulaCell::getFormula() const {
    return formulaValue;
}
// Returns the string representation of the formula for display purposes
string FormulaCell::getDisplayValue() const {
    return formulaValue; // Assuming that the formula is stored as a string representation.
}
// Evaluates the formula and returns the numeric value (placeholder implementation)
double FormulaCell::value() const {
    return 0.0; // Placeholder: could throw an exception or return NaN for unsupported operations
}
// Creates a deep copy of the current FormulaCell object
std::unique_ptr<Cell> FormulaCell::clone() const {
    return std::make_unique<FormulaCell>(*this);  
}

// IntValueCell: Represents a cell containing an integer value
IntValueCell::IntValueCell(int value) : ValueCell(), intValue(value) {}

// Sets a new integer value for the cell
void IntValueCell::setValue(int value) {
    intValue = value;
}
// Returns the integer value stored in the cell
int IntValueCell::getValue() const {
    return intValue;
}
// Converts the integer value to a string for display purposes
string IntValueCell::getDisplayValue() const {
    stringstream ss;
    ss << intValue;
    return ss.str();
}
// Returns the integer value as a double
double IntValueCell::value() const {
    return static_cast<double>(intValue); // Return the integer as a double
}
// Creates a deep copy of the current IntValueCell object
std::unique_ptr<Cell> IntValueCell::clone() const {
    return std::make_unique<IntValueCell>(*this);  // Clone the current IntValueCell object
}

// DoubleValueCell: Represents a cell containing a double value
DoubleValueCell::DoubleValueCell(double value) : ValueCell(), doubleValue(value) {}

// Sets a new double value for the cell
void DoubleValueCell::setValue(double value) {
    doubleValue = value;
}
// Returns the double value stored in the cell
double DoubleValueCell::getValue() const {
    return doubleValue;
}
// Converts the double value to a string for display purposes
string DoubleValueCell::getDisplayValue() const {
    stringstream ss;
    ss << doubleValue;
    return ss.str();
}
// Returns the double value
double DoubleValueCell::value() const {
    return doubleValue; // Return the double value
}
// Creates a deep copy of the current DoubleValueCell object
std::unique_ptr<Cell> DoubleValueCell::clone() const {
    return std::make_unique<DoubleValueCell>(*this);  
}

// StringValueCell: Represents a cell containing a string value
StringValueCell::StringValueCell(const string& value) : ValueCell(), strValue(value) {}

// Sets a new string value for the cell
void StringValueCell::setValue(const string& value) {
    strValue = value;
}
// Returns the string value stored in the cell
string StringValueCell::getValue() const {
    return strValue;
}
// Returns the string value for display purposes
string StringValueCell::getDisplayValue() const {
    return strValue;
}
// Evaluates the string value and returns 0.0 for non-numeric cells
double StringValueCell::value() const {
    return 0.0; // Placeholder: could throw an exception or return NaN for unsupported operations
}
// Creates a deep copy of the current StringValueCell object
std::unique_ptr<Cell> StringValueCell::clone() const {
    return std::make_unique<StringValueCell>(*this);  
}
