#include "AnsiTerminal.h"
#include "Spreadsheet.h"
#include "FormulaParser.h"
#include "FileManager.h"
#include <iostream>
#include <string>
using namespace std;


int main() {
    AnsiTerminal terminal; // Create an instance of the terminal class

    terminal.clearScreen(); // Clear the screen at the beginning

    Spreadsheet sheet(100, 100); // Create a 100x100 spreadsheet


    int row = 1, col = 1;  // Initial position of the cursor
    int maxDisplayRows = 20; // Maximum number of rows to display on screen
    int maxDisplayCols = 80;  // Maximum number of rows to display on screen

    // Maximum number of rows to display on screen
    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);
    

    char key;
    string input;

    // Maximum number of rows to display on screen
    while (true) {
        
        terminal.moveCursor(row + 3, col * 10);
        
        
        key = terminal.getSpecialKey();

        // Clear the previous position
       

        // Update position based on arrow key input
        switch (key) {
            case 'U': row = (row > 1) ? row - 1 : row; break; // Up
            case 'D': row = (row < 20) ? row + 1 : row; break; // Down
            case 'R': col = (col < 80) ? col + 1 : col; break; // Right
            case 'L': col = (col > 1) ? col - 1 : col; break; // Left
            case 'q': return 0; // Quit program if 'q' is pressed
            case 's': {
                terminal.moveCursor(25, 1);  
                std::cout << "Enter filename to save the spreadsheet: ";
                std::string saveFilename;
                char ch;
                
                // Read characters one by one
                while (true) {
                    ch = std::cin.get();  // Read one character at a time

                    // Stop when the user presses Enter (newline character)
                    if (ch == '\n') {
                        break;
                    }

                    // Display the character immediately after it is entered
                    std::cout << ch; 

                    // Append the character to the filename string
                    saveFilename += ch;
                }

                 // Save the spreadsheet to the specified file
                FileManager::saveToCSV(sheet, saveFilename);
                 // Reload the spreadsheet from the same file
                FileManager::loadFromCSV(sheet, saveFilename);
                break;
            } //save spreadsheet if 's' is pressed
            case 'l': {
                terminal.moveCursor(25, 1);  
                std::cout << "Enter filename to load the spreadsheet: ";
                std::string loadFilename;
                char ch;

                // Read characters one by one
                while (true) {
                ch = std::cin.get();  // Read one character at a time

                // Stop when the user presses Enter (newline character)
                if (ch == '\n') {
                    break;
                }

                // Display the character immediately after it is entered
                std::cout << ch;

                // Append the character to the filename string
                loadFilename += ch;
                }

                // Call loadFromCSV with the user-provided filename
                FileManager::loadFromCSV(sheet, loadFilename);
                sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);
                break;
            } // Load spreadsheet if 'l' is pressed
            case 'E': // Enter cell data if 'E' is pressed
                terminal.moveCursor(1, 1); // Move to the space above the table
                 cout << sheet.getCellReference(row, col); // Display the cell reference (e.g., A1:)
                terminal.moveCursor(2, 1);
                string input;
                char ch;

                // Capture input character by character
                while (true) {
                ch = cin.get();  // Read one character at a time
                if (ch == '\n') {  // If Enter is pressed, break out of the loop
                break;
                }

                // Echo the character as it is typed
                cout << ch;
                // Add the character to the input string
                input += ch;
                }

                // After input is done, process the input
                sheet.enterCellData(row, col, input);  // Store the data in the appropriate cell
                sheet.recalculate();  // Recalculate if necessary
                sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Display the updated spreadsheet
                    break;

        }

    }    // Display inverted cursor at new position
       
    

    terminal.clearScreen(); // Clear the screen on exit
    return 0;
}


