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
		string getDisplayValue() const override;
		double value() const override;
		std::unique_ptr<Cell> clone() const override;
	private:
		string formulaValue; // Stores the formula as a string
};

// Abstract base class for value-based cells
class ValueCell : public Cell {
    public:
        ValueCell() : recalculate(false) {}  // Initialize recalculate member variable
        virtual string getDisplayValue() const = 0;
    protected:
        bool recalculate;  // Member variable for recalculation flag
};

// Class representing an integer value cell
class IntValueCell : public ValueCell {
	public:
		explicit IntValueCell(int value = 0);
		void setValue(int value);
		int getValue() const;
		string getDisplayValue() const override;
		double value() const override;
		std::unique_ptr<Cell> clone() const override;
	private:
		int intValue; // Stores the integer value
};

// Class representing a double value cell
class DoubleValueCell : public ValueCell {
	public:
		explicit DoubleValueCell(double value = 0.0);
		void setValue(double value);
		double getValue() const;
		string getDisplayValue() const override;
		double value() const override;
		std::unique_ptr<Cell> clone() const override;
	private:
		double doubleValue; // Stores the double value
};

// Class representing a string value cell
class StringValueCell : public ValueCell {
	public:
		explicit StringValueCell(const string& value);
		void setValue(const string& value);
		string getValue() const;
		string getDisplayValue() const override;
		double value() const override;
		std::unique_ptr<Cell> clone() const override;
	private:
		string strValue; // Stores the string value
};

#endif // CELL_H