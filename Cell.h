#ifndef CELL_H
#define CELL_H

#include <string>
#include <iostream>
#include <sstream>
#include <memory>
using namespace std;

class Cell {
	public:
		virtual ~Cell() = default;
		virtual string getDisplayValue() const = 0;
		virtual double value() const = 0;
		virtual std::unique_ptr<Cell> clone() const = 0;
};

class FormulaCell : public Cell {
	public:
		FormulaCell(const string& formula);
		void setFormula(const string& formula);
		string getFormula() const ;
		string getDisplayValue() const;
		double value() const;
		std::unique_ptr<Cell> clone() const;

	private:
		string formulaValue;
};


class ValueCell : public Cell {
    public:
        ValueCell() : recalculate(false) {}  // Initialize recalculate member variable
        virtual string getDisplayValue() const = 0;

    protected:
        bool recalculate;  // Member variable for recalculation flag
};


class IntValueCell : public ValueCell {
	public:
		IntValueCell(int value = 0);
		void setValue(int value);
		int getValue() const;
		string getDisplayValue() const;
		double value() const;
		std::unique_ptr<Cell> clone() const;

	private:
		int intValue;
};

class DoubleValueCell : public ValueCell {
	public:
		DoubleValueCell(double value = 0.0);
		void setValue(double value);
		double getValue() const;
		string getDisplayValue() const;
		double value() const;
		std::unique_ptr<Cell> clone() const;
	private:
		double doubleValue;
};

class StringValueCell : public ValueCell {
	public:
		StringValueCell(const string& value);
		void setValue(const string& value);
		string getValue() const;
		string getDisplayValue() const;
		double value() const;
		std::unique_ptr<Cell> clone() const;
	private:
		string strValue;
};

#endif