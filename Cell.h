#ifndef CELL_H
#define CELL_H

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
using namespace std;

// Abstract base class representing a generic cell
class Cell {
	public:
		virtual ~Cell() = default; // Virtual destructor for proper cleanup in derived classes
		virtual string getDisplayValue() const = 0; // Returns the value of the cell as a string for display purposes
		virtual double value() const = 0; // Returns the numerical value of the cell (if applicable)
		virtual std::unique_ptr<Cell> clone() const = 0;  // Clones the current cell, returning a unique_ptr to the new instance
};

// Class representing a formula cell
class FormulaCell : public Cell {
	public:
		FormulaCell(const string& formula); // Constructor to initialize with a formula
		void setFormula(const string& formula); // Sets a new formula for the cell
		string getFormula() const ; // Gets the stored formula
		string getDisplayValue() const; // Returns the formula as a string for display purposes
		double value() const; // Evaluates the formula and returns its numeric value (placeholder implementation)
		std::unique_ptr<Cell> clone() const; // Clones the current FormulaCell instance
	private:
		string formulaValue; // Stores the formula as a string
};

// Abstract base class for value-based cells
class ValueCell : public Cell {
    public:
        ValueCell() : recalculate(false) {}  // Initialize recalculate member variable
        virtual string getDisplayValue() const = 0;  // Overridden by derived classes to return the cell value as a string
    protected:
        bool recalculate;  // Member variable for recalculation flag
};

// Class representing an integer value cell
class IntValueCell : public ValueCell {
	public:
		IntValueCell(int value = 0); // Constructor with default value of 0
		void setValue(int value); // Sets a new integer value for the cell
		int getValue() const; // Returns the stored integer value
		string getDisplayValue() const; // Returns the integer value as a string
		double value() const; // Returns the integer value as a double
		std::unique_ptr<Cell> clone() const; // Clones the current IntValueCell instance
	private:
		int intValue; // Stores the integer value
};

// Class representing a double value cell
class DoubleValueCell : public ValueCell {
	public:
		DoubleValueCell(double value = 0.0); // Constructor with default value of 0.0
		void setValue(double value); // Sets a new double value for the cell
		double getValue() const; // Returns the stored double value
		string getDisplayValue() const; // Returns the double value as a string
		double value() const; // Returns the double value
		std::unique_ptr<Cell> clone() const; // Clones the current DoubleValueCell instance
	private:
		double doubleValue; // Stores the double value
};

// Class representing a string value cell
class StringValueCell : public ValueCell {
	public:
		StringValueCell(const string& value); // Constructor to initialize with a string value
		void setValue(const string& value); // Sets a new string value for the cell
		string getValue() const; // Returns the stored string value
		string getDisplayValue() const; // Returns the string value for display purposes
		double value() const; // Returns a numerical representation of the string value (default 0.0)
		std::unique_ptr<Cell> clone() const; // Clones the current StringValueCell instance
	private:
		string strValue; // Stores the string value
};

#endif // CELL_H