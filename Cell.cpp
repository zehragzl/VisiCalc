#include "Cell.h"
using namespace std;
#include <string>

double Cell::value() const {
    // If the cell is of type IntValueCell, return the integer value
    if (const IntValueCell* intCell = dynamic_cast<const IntValueCell*>(this)) {
        return static_cast<double>(intCell->getValue());  // Cast the integer to double
    }
    
    // If the cell is of type DoubleValueCell, return the double value
    if (const DoubleValueCell* doubleCell = dynamic_cast<const DoubleValueCell*>(this)) {
        return doubleCell->getValue();  // Return the double value
    }

    // If the cell type is not recognized (e.g., String, Formula, or Empty), return 0
    return 0.0;
}
// FormulaCell Implementation
FormulaCell::FormulaCell(const string& formula="") : formulaValue(formula) {
	recalculate = true;
}
void FormulaCell::setFormula(const string& formula){
	formulaValue = formula;
	recalculate = true;

	//updateDependencies(this);
}
string FormulaCell::getFormula() const {return formulaValue;}
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
int  IntValueCell::getValue() const {return intValue;}

string IntValueCell::getDisplayValue() const  {
	return to_string(intValue);
}

// DoubleValueCell Implementation
DoubleValueCell::DoubleValueCell(double value) : ValueCell(Cell::ValueType::NUMBER), doubleValue(value) {
}

void DoubleValueCell::setValue(double value) {
	doubleValue = value;
}
double DoubleValueCell::getValue() const {return doubleValue;}
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
string StringValueCell:: getValue() const {return strValue;}

string StringValueCell::getDisplayValue() const  {
	return strValue;
}
