#include "Cell.h"

// FormulaCell Implementation
FormulaCell::FormulaCell(const string& formula="") : formulaValue(formula) {
	recalculate = true;
}
void FormulaCell::setFormula(const string& formula){
	formulaValue = formula;
	recalculate = true;

	//updateDependencies(this);
}
string FormulaCell::getDisplayValue() const  {
	return formulaValue;
}
Cell::ValueType FormulaCell:: getType() const  {
	return ValueType::FORMULA;
}
void FormulaCell::setNeedsRecalculation(bool needsRecalculation) {
    recalculate = needsRecalculation;
}

bool FormulaCell::needsRecalculation() const {
    return recalculate;
}

// ValueCell Implementation

ValueCell::ValueCell(ValueType t) : type(t), recalculate(false) {}

Cell::ValueType ValueCell::getType() const {
    return type;
}

void ValueCell::setNeedsRecalculation(bool needsRecalculation) {
    recalculate = needsRecalculation;
}

bool ValueCell::needsRecalculation() const {
    return recalculate;
}

// IntValueCell Implementation
IntValueCell::IntValueCell(int value) : ValueCell(Cell::ValueType::NUMBER), intValue(value) {
}

void IntValueCell::setValue(int value) {
	intValue = value;
}

string IntValueCell::getDisplayValue() const  {
	return to_string(intValue);
}

// DoubleValueCell Implementation
DoubleValueCell::DoubleValueCell(double value) : ValueCell(Cell::ValueType::NUMBER), doubleValue(value) {
}

void DoubleValueCell::setValue(double value) {
	doubleValue = value;
}
string DoubleValueCell::getDisplayValue() const  {
	std::ostringstream oss;
    oss << doubleValue;
    return oss.str();
}

// StringValueCell Implementation
StringValueCell::StringValueCell(const string& value = "") : ValueCell(Cell::ValueType::STRING), strValue(value) {
}

void StringValueCell::setValue(const string& value) {
	strValue = value;
}

string StringValueCell::getDisplayValue() const  {
	return strValue;
}
