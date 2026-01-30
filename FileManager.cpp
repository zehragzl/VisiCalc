#include "FileManager.h"
#include "Spreadsheet.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Spreadsheet {

void FileManager::saveToCSV(const Spreadsheet& spreadsheet, const std::string& filename) {
    std::ofstream file(filename); // Open file for writing
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

     // Iterate over rows and columns of the spreadsheet
    for (int row = 1; row <= spreadsheet.getRowCount(); ++row) {
        for (int col = 1; col <= spreadsheet.getColCount(); ++col) {
            std::string content;
            try {
                // Retrieve the content of the cell as a string
                content = spreadsheet.getCellContent<std::string>(row, col);
            } catch (const std::runtime_error&) {
                content = " "; // Default value for empty cells
            }
            file << content; // Write cell content to file

            if (col < spreadsheet.getColCount()) {
                file << ","; // Add a comma to separate cells
            }
        }
        file << "\n"; // Move to the next row
    }
    file.close(); // Close the file after writing
}

void FileManager::loadFromCSV(Spreadsheet& spreadsheet, const std::string& filename) {
    std::ifstream file(filename); // Open file for reading
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 1;

    // Read the file line by line
    while (std::getline(file, line)) {
        std::stringstream lineStream(line);
        std::string cellValue;
        int col = 1;

        // Split the line into cells by commas
        while (std::getline(lineStream, cellValue, ',')) {
            // Trim surrounding quotes (if any) from cell value
            if (!cellValue.empty() && cellValue[0] == '"') {
                cellValue = cellValue.substr(1, cellValue.size() - 2); // Remove quotes
            }

            try {
                // Determine the type of cell content and create the appropriate cell
                if (!cellValue.empty() && std::isdigit(cellValue[0])) {
                    // Check if the value is a double or an int
                    if (cellValue.find('.') != std::string::npos) {
                        double doubleValue = std::stod(cellValue);
                        spreadsheet.setCell(row, col, std::make_unique<DoubleValueCell>(doubleValue));
                    } else {
                        int intValue = std::stoi(cellValue);
                        spreadsheet.setCell(row, col, std::make_unique<IntValueCell>(intValue));
                    }
                } else if (!cellValue.empty() && cellValue[0] == '=') {
                    // If the value starts with '=', treat it as a formula
                    spreadsheet.setCell(row, col, std::make_unique<FormulaCell>(cellValue));
                } else {
                    // Treat the value as a string
                    spreadsheet.setCell(row, col, std::make_unique<StringValueCell>(cellValue));
                }
            } catch (const std::exception& e) {
                std::cerr << "Error processing cell at (" << row << ", " << col << "): " << e.what() << std::endl;
            }

            ++col; // Move to the next column
        }

        ++row; // Move to the next row
    }

    file.close(); // Close the file after reading
}
} // namespace Spreadsheet
