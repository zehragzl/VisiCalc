#include "FormulaParser.h"
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <numeric>
#include <regex>

// Constructor: Initializes the formula parser with rows, columns, and cell values
FormulaParser::FormulaParser(int rows, int cols, std::unordered_map<std::string, double> cellValues)
    : rows(rows), cols(cols), cellValues(std::move(cellValues)){}








// Function to evaluate a mathematical expression, replacing cell references with actual values
double FormulaParser::evaluateExpression(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::string modifiedExpr = expr;

    // Replace all cell references in the expression with their respective values
    for (const std::pair<std::string, double>& pair : cellValues) {
        const std::string& cellRef = pair.first;
        double value = pair.second;

        size_t pos = 0;
        while ((pos = modifiedExpr.find(cellRef, pos)) != std::string::npos) {
            modifiedExpr.replace(pos, cellRef.length(), std::to_string(value));
            pos += std::to_string(value).length();
        }
    }

    // Check if there are any unresolved cell references
    for (const std::pair<std::string, double>& pair : cellValues) {
        const std::string& cellRef = pair.first;
        if (modifiedExpr.find(cellRef) != std::string::npos && cellValues.find(cellRef) == cellValues.end()) {
            throw std::runtime_error("Cell reference " + cellRef + " is empty or missing.");
        }
    }

    // Now, evaluate the modified expression
    std::istringstream stream(modifiedExpr);
    double result = 0.0;
    stream >> result;  // Get the first number

    char op;
    double num;
    while (stream >> op >> num) {
        switch (op) {
            case '+': result += num; break;
            case '-': result -= num; break;
            case '*': result *= num; break;
            case '/': 
                if (num == 0.0) throw std::runtime_error("Division by zero");
                result /= num; 
                break;
            default:
                throw std::runtime_error("Unsupported operator: " + std::string(1, op));
        }
    }

    return result;
}
