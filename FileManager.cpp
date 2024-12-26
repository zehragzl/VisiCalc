#include "FileManager.h"
#include "Spreadsheet.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Spreadsheet {

void FileManager::saveToCSV(const Spreadsheet& spreadsheet, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    for (int row = 1; row <= spreadsheet.getRowCount(); ++row) {
        for (int col = 1; col <= spreadsheet.getColCount(); ++col) {
            std::string content;
            try {
                content = spreadsheet.getCellContent<std::string>(row, col);
            } catch (const std::runtime_error&) {
                content = " "; // Varsayılan boş değer
            }

            file << content; // Enclose the content in quotes to handle commas

            if (col < spreadsheet.getColCount()) {
                file << ","; // Add a comma to separate cells
            }
        }
        file << "\n"; // Move to the next row
    }

    file.close();
}

void FileManager::loadFromCSV(Spreadsheet& spreadsheet, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 1;

    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cellValue;
        int col = 1;

        while (std::getline(lineStream, cellValue, ',')) {
            // Trim any surrounding spaces or quotes from cell value
            if (!cellValue.empty() && cellValue[0] == '"') {
                cellValue = cellValue.substr(1, cellValue.size() - 2); // Remove quotes
            }

            try {
                // Check if the cellValue is a number (int or double)
                if (!cellValue.empty() && std::isdigit(cellValue[0])) {
                    if (cellValue.find('.') != std::string::npos) {
                        double doubleValue = std::stod(cellValue);
                        spreadsheet.setCell(row, col, std::make_unique<DoubleValueCell>(doubleValue));
                    } else {
                        int intValue = std::stoi(cellValue);
                        spreadsheet.setCell(row, col, std::make_unique<IntValueCell>(intValue));
                    }
                } else if (!cellValue.empty() && cellValue[0] == '=') {
                    // Treat as formula
                    spreadsheet.setCell(row, col, std::make_unique<FormulaCell>(cellValue));
                } else {
                    // Treat as string
                    spreadsheet.setCell(row, col, std::make_unique<StringValueCell>(cellValue));
                }
            } catch (const std::exception& e) {
                std::cerr << "Error processing cell at (" << row << ", " << col << "): " << e.what() << std::endl;
            }

            ++col;
        }

        ++row;
    }

    file.close();
}



} // namespace Spreadsheet
