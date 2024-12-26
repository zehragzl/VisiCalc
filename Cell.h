#ifndef CELL_H
#define CELL_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Cell {
	public:
	 // Enum class to define possible types of values a cell can hold
	enum class ValueType{
			EMPTY,
			NUMBER,
			STRING,
			FORMULA
		};
		
		virtual ~Cell() = default;
		
		virtual ValueType getType() const = 0; 
		virtual string getDisplayValue() const = 0;
		virtual void setNeedsRecalculation(bool needsRecalculation) = 0;
		virtual bool needsRecalculation() const = 0;

};

class FormulaCell : public Cell{
	public:
		FormulaCell(const string& formula);
		void setFormula(const string& formula);
		string getFormula() const ;
		string getDisplayValue() const;
		ValueType getType() const ;
		void setNeedsRecalculation(bool needsRecalculation) ;
    	bool needsRecalculation() const ;
	
	private:
		string formulaValue;
		bool recalculate;
};


class ValueCell : public Cell{
	public:
		ValueCell(ValueType t);
		ValueType getType() const ;
		void setNeedsRecalculation(bool needsRecalculation) ;
    	bool needsRecalculation() const ;

	protected:
		ValueType type = ValueType::EMPTY;
		bool recalculate;
};


class IntValueCell : public ValueCell{
	public:
		IntValueCell(int value = 0);
		void setValue(int value);
		int getValue() const;
		string getDisplayValue() const ;

	private:
		int intValue;
};

class DoubleValueCell : public ValueCell{
	public:
		DoubleValueCell(double value = 0.0);
		void setValue(double value);
		double getValue() const;
		string getDisplayValue() const ;

	private:
		double doubleValue;
};

class StringValueCell : public ValueCell{
	public:
		StringValueCell(const string& value);
		void setValue(const string& value);
		string getValue() const;
		string getDisplayValue() const ;

	private:
		string strValue;
};



#endif