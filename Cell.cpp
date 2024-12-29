#include "Cell.h"

// FormulaCell
FormulaCell::FormulaCell(const string& formula) : formulaValue(formula) {}

void FormulaCell::setFormula(const string& formula) {
    formulaValue = formula;
}

string FormulaCell::getFormula() const {
    return formulaValue;
}

string FormulaCell::getDisplayValue() const {
    return formulaValue; // Assuming that the formula is stored as a string representation.
}
double FormulaCell::value() const {
    return 0.0; // Assuming non-numeric string cells return 0.0 (could throw exception or NaN instead)
}
std::unique_ptr<Cell> FormulaCell::clone() const {
    return std::make_unique<FormulaCell>(*this);  // Clone the current FormulaCell object
}

// IntValueCell
IntValueCell::IntValueCell(int value) : ValueCell(), intValue(value) {}


void IntValueCell::setValue(int value) {
    intValue = value;
}

int IntValueCell::getValue() const {
    return intValue;
}

string IntValueCell::getDisplayValue() const {
    stringstream ss;
    ss << intValue;
    return ss.str();
}
double IntValueCell::value() const {
    return static_cast<double>(intValue); // Return the integer as a double
}
std::unique_ptr<Cell> IntValueCell::clone() const {
    return std::make_unique<IntValueCell>(*this);  // Clone the current IntValueCell object
}

// DoubleValueCell
DoubleValueCell::DoubleValueCell(double value) : ValueCell(), doubleValue(value) {}

void DoubleValueCell::setValue(double value) {
    doubleValue = value;
}

double DoubleValueCell::getValue() const {
    return doubleValue;
}

string DoubleValueCell::getDisplayValue() const {
    stringstream ss;
    ss << doubleValue;
    return ss.str();
}
double DoubleValueCell::value() const {
    return doubleValue; // Return the double value
}
std::unique_ptr<Cell> DoubleValueCell::clone() const {
    return std::make_unique<DoubleValueCell>(*this);  // Clone the current DoubleValueCell object
}

// StringValueCell
StringValueCell::StringValueCell(const string& value) : ValueCell(), strValue(value) {}

void StringValueCell::setValue(const string& value) {
    strValue = value;
}

string StringValueCell::getValue() const {
    return strValue;
}

string StringValueCell::getDisplayValue() const {
    return strValue;
}

double StringValueCell::value() const {
    return 0.0; // Assuming non-numeric string cells return 0.0 (could throw exception or NaN instead)
}
std::unique_ptr<Cell> StringValueCell::clone() const {
    return std::make_unique<StringValueCell>(*this);  // Clone the current StringValueCell object
}
