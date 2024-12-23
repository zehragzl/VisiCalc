#include "FileManager.h"
#include "Spreadsheet.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cctype>

// Saves the spreadsheet content to a CSV file with the specified filename
void FileManager:: saveToCSV(const Spreadsheet& spreadsheet, const std::string& filename){
 std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        // Get the number of rows and columns from the spreadsheet
        int rows = spreadsheet.getRows();
        int cols = spreadsheet.getCols();

        // Iterate through each cell in the spreadsheet
        for (int row = 1; row <= rows; ++row) {
            for (int col = 1; col <= cols; ++col) {
                const Cell& cell = spreadsheet.getCells()[row - 1][col - 1];
                std::string cellContent = cell.getDisplayValue(); // Get the display value (whether it's a number or string)
                
                if (cell.getType() == Cell::ValueType::FORMULA) {
                    // If it's a formula, store it as a string (with '=')
                    cellContent = cell.getFormula(); 
                }
                
                file << cellContent;
                
                if (col < cols) {
                    file << ","; 
                }
            }
            file << "\n"; 
        }

        file.close();

        std::cout << " Spreadsheet saved to: " << filename << std::endl;
    }

// Loads the spreadsheet content from a CSV file
void FileManager::loadFromCSV(Spreadsheet &spreadsheet, const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file for reading: " << filename << std::endl;
        return;
    }

    std::string line;
    int row = 1;

    // Read the file line by line
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        int col = 1;

        // Read each value separated by a comma
        while (std::getline(ss, value, ',') && col <= spreadsheet.getCols()) {
            // Set the value in the corresponding spreadsheet cell
            spreadsheet.setCellContent(row, col, value);
            ++col;
        }

        ++row;
        if (row > spreadsheet.getRows()) break;  // Stop if we've filled the row limit
    }

    file.close();
    std::cout << "Spreadsheet loaded from: " << filename << std::endl;
}
