#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Cell.h"
#include "FormulaParser.h"  
#include <string>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
using namespace std;

namespace Spreadsheet{

	class OutOfBounds : public runtime_error {
		public:
			OutOfBounds(const string& message) : runtime_error(message) {}
	};


	class InvalidFormula : public runtime_error {	
		public:
			InvalidFormula(const string& message) : runtime_error(message) {}
	};


	template <class T>
	class Grid {
		public:
			Grid(int rows, int cols);
			T& at(int roe, int col);
			const T& at(int row, int col) const;

		private:
			int rows, cols;
			unique_ptr<unique_ptr<T[]>[]> grid;

	};

	class Spreadsheet {
	public:
	    // Constructor to initialize the spreadsheet with a specific number of rows and columns
		Spreadsheet(int rows, int cols);

		Cell* getCell(int row, int col) const;

		// Getter methods for rows and columns count
		int getRows() const;
    	int getCols() const;

		//Validation method to check if a given cell (row, col) is within bounds
		bool isValidCell(int row, int col) const;

		// Method to input data into a specific cell
		void enterCellData(int row, int col, const string &input);

		// Setter methods for setting content in a specific cell
		void setCellContent(int row, int col, const string &content);

    	// Getter method to retrieve content from a specific cell
		 template <typename T>
    	T getCellContent(int row, int col) const;

	    // Method to retrieve the reference (e.g., A1, B2) of a specific cell	
    	std::string getCellReference(int row, int col) const;

		// Method to display the spreadsheet with a specific max number of rows and columns
		void displaySpreadsheet(int maxRows, int maxCols) const;

		// Method to recalculate the entire spreadsheet (for formulas that depend on other cells)
		void recalculate();

		// Methods to evaluate various functions (SUM, AVERAGE, STDDEV, MAX, MIN) in a formula
		/*double evaluateSum(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
		double evaluateAverage(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
		double evaluateStdDev(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
		double evaluateMax(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
		double evaluateMin(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);
		*/	

	private:
		Grid<Cell> grid;
		int rows, cols;
    	FormulaParser formulaParser;  // Instance of the FormulaParser class to parse and evaluate formulas
		
		// Helper method to evaluate a formula for a specific cell
	//	double evaluateFormula(Cell& cell, const std::string& currentCell, std::unordered_set<std::string>& processing);	
	};

}
#include "Spreadsheet.tpp"

#endif

/*void Spreadsheet::updateDependencies(Cell* cell) {
    auto dependents = dependencies[cell->getReference()];
    for (auto& dependent : dependents) {
        auto dependentCell = getCell(dependent);
        dependentCell->setNeedsRecalculation(true);
        dependentCell->recalculate();
    }
}*/