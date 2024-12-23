#ifndef FORMULAPARSER_H
#define FORMULAPARSER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <functional>

class FormulaParser {
public:
    // Constructor: Initializes the formula parser with rows, columns, and cell values
    FormulaParser(int rows, int cols, std::unordered_map<std::string, double> cellValues);
    
    // Main function to evaluate an expression
    double evaluateExpression(const std::string& expr, const std::unordered_map<std::string, double>& cellValues);


private:
    int rows; 
    int cols; 
    std::unordered_map<std::string, double> cellValues;  // A map of cell references to values
};

#endif