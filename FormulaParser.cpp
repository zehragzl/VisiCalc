#include "FormulaParser.h"
#include <sstream>
#include <iostream>
#include <regex>
#include <cmath>
#include <vector>

namespace Spreadsheet {

FormulaParser::FormulaParser(int rows, int cols, std::unordered_map<std::string, double> cellValues)
    : rows(rows), cols(cols), cellValues(std::move(cellValues)) {}

double FormulaParser::evaluateExpression(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::string modifiedExpr = expr;

    // Replace all cell references in the expression with their respective values
    for (const auto& pair : cellValues) {
        const std::string& cellRef = pair.first;
        double value = pair.second;

        size_t pos = 0;
        while ((pos = modifiedExpr.find(cellRef, pos)) != std::string::npos) {
            modifiedExpr.replace(pos, cellRef.length(), std::to_string(value));
            pos += std::to_string(value).length();
        }
    }

    std::istringstream stream(modifiedExpr);
    double result = 0.0;
    stream >> result;

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

double FormulaParser::evaluateFormula(const std::string& formula, std::unordered_set<std::string>& processing) {
    if (processing.find(formula) != processing.end()) {
        throw std::runtime_error("Cyclic dependency detected in formula: " + formula);
    }

    processing.insert(formula);

    std::unordered_map<std::string, double> cellValues;
    for (const auto& pair : this->cellValues) {
        cellValues[pair.first] = pair.second;
    }

    double result = 0.0;
    try {
        if (formula.find("SUM") == 0) {
            result = evaluateSum(formula, cellValues);
        }else if (formula.find("AVER") == 0) {
            result = evaluateAverage(formula, cellValues);
        } else if (formula.find("STDDEV") == 0) {
            result = evaluateStdDev(formula, cellValues);
        } else if (formula.find("MAX") == 0) {
            result = evaluateMax(formula, cellValues);
        } else if (formula.find("MIN") == 0) {
            result = evaluateMin(formula, cellValues);
        } else {
            // Handle regular arithmetic expressions
            result = evaluateExpression(formula, cellValues);
        }
    } catch (const std::exception& e) {
        processing.erase(formula);
        throw std::runtime_error("Error evaluating formula: " + std::string(e.what()));
    }

    processing.erase(formula);
    return result;
}

double FormulaParser::evaluateSum(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("SUM\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        int startRow, startCol, endRow, endCol;
        extractCellRange(expr, startRow, startCol, endRow, endCol);

        double sum = 0.0;
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = indexToCell(r, c);  // Convert index back to cell reference (e.g., "A1")
                if (cellValues.find(cellRef) != cellValues.end()) {
                    sum += cellValues.at(cellRef);
                }
            }
        }

        return sum;
    }

    throw std::runtime_error("Invalid SUM expression");
}

double FormulaParser::evaluateAverage(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("AVER\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        int startRow, startCol, endRow, endCol;
        extractCellRange(expr, startRow, startCol, endRow, endCol);

        double sum = 0.0;
        int count = 0;
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = indexToCell(r, c);
                if (cellValues.find(cellRef) != cellValues.end()) {
                    sum += cellValues.at(cellRef);
                    ++count;
                }
            }
        }

        if (count == 0) throw std::runtime_error("Cannot compute average: no values in range");
        return sum / count;
    }

    throw std::runtime_error("Invalid AVER expression");
}

double FormulaParser::evaluateStdDev(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("STDDEV\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        int startRow, startCol, endRow, endCol;
        extractCellRange(expr, startRow, startCol, endRow, endCol);

        std::vector<double> values;
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = indexToCell(r, c);
                if (cellValues.find(cellRef) != cellValues.end()) {
                    values.push_back(cellValues.at(cellRef));
                }
            }
        }

        if (values.size() < 2) throw std::runtime_error("Cannot compute standard deviation: not enough values");

        double mean = 0.0;
        for (double value : values) {
            mean += value;
        }
        mean /= values.size();

        double sumSqDiffs = 0.0;
        for (double value : values) {
            sumSqDiffs += (value - mean) * (value - mean);
        }

        return std::sqrt(sumSqDiffs / values.size());
    }

    throw std::runtime_error("Invalid STDDEV expression");
}

double FormulaParser::evaluateMax(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("MAX\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        int startRow, startCol, endRow, endCol;
        extractCellRange(expr, startRow, startCol, endRow, endCol);

        double max = -std::numeric_limits<double>::infinity();
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = indexToCell(r, c);
                if (cellValues.find(cellRef) != cellValues.end()) {
                    max = std::max(max, cellValues.at(cellRef));
                }
            }
        }

        return max;
    }

    throw std::runtime_error("Invalid MAX expression");
}

double FormulaParser::evaluateMin(const std::string& expr, const std::unordered_map<std::string, double>& cellValues) {
    std::regex rangeRegex("MIN\\(([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)\\)");
    std::smatch match;

    if (std::regex_search(expr, match, rangeRegex)) {
        int startRow, startCol, endRow, endCol;
        extractCellRange(expr, startRow, startCol, endRow, endCol);

        double min = std::numeric_limits<double>::infinity();
        for (int r = startRow; r <= endRow; ++r) {
            for (int c = startCol; c <= endCol; ++c) {
                std::string cellRef = indexToCell(r, c);
                if (cellValues.find(cellRef) != cellValues.end()) {
                    min = std::min(min, cellValues.at(cellRef));
                }
            }
        }

        return min;
    }

    throw std::runtime_error("Invalid MIN expression");
}

int FormulaParser::columnToIndex(const std::string& col) {
    int index = 0;
    for (char c : col) {
        index = index * 26 + (c - 'A');
    }
    return index;
}

std::string FormulaParser::indexToCell(int row, int col) {
    std::string cellRef;
    while (col >= 0) {
        cellRef = char('A' + col % 26) + cellRef;
        col /= 26;
        --col;  // Adjust for 1-indexed behavior
    }
    return cellRef + std::to_string(row + 1);  // Add 1 for 1-indexed row
}

void FormulaParser::extractCellRange(const std::string& range, int& startRow, int& startCol, int& endRow, int& endCol) {
    std::smatch match;
    std::regex rangeRegex("([A-Z]+[0-9]+)\\.\\.([A-Z]+[0-9]+)");

    if (std::regex_search(range, match, rangeRegex)) {
        // Extract start cell (e.g., "A1")
        startRow = std::stoi(match[1].str().substr(1)) - 1; // Adjust row to 0-indexed
        startCol = columnToIndex(match[1].str().substr(0, match[1].str().length() - 1));

        // Extract end cell (e.g., "A3")
        endRow = std::stoi(match[2].str().substr(1)) - 1; // Adjust row to 0-indexed
        endCol = columnToIndex(match[2].str().substr(0, match[2].str().length() - 1));
    }
}
} // namespace Spreadsheet
