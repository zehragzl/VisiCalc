#include "AnsiTerminal.h"
#include "Spreadsheet.h"
#include "FileManager.h"
#include <iostream>
#include <string>

using namespace std;
using namespace Spreadsheet;

int main() {
    AnsiTerminal terminal;  // Create an instance of the terminal class
    terminal.clearScreen(); // Clear the screen at the beginning

    Spreadsheet::Spreadsheet sheet(100, 100); // Create a spreadsheet with 20 rows and 20 columns
    int row = 1, col = 1;  // Initial position of the cursor
    int maxDisplayRows = 20;  // Maximum number of rows to display on screen
    int maxDisplayCols = 80;  // Maximum number of columns to display on screen

    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols); // Display the initial state of the spreadsheet

    char key; // Variable for key input
    string input; // String to hold input for cell data

    // Main loop to handle user input and actions
    while (true) {
        terminal.moveCursor(row + 3, col * 10);  // Move cursor to the correct position
        key = terminal.getSpecialKey();  // Get key input

        switch (key) {
            case 'G': {  // Adjust display dimensions if 'G' is pressed
                terminal.moveCursor(1, 1);
                cout << "Enter new number of display rows: ";
                int newRows;
                cin >> newRows;

                cout << "Enter new number of display columns: ";
                int newCols;
                cin >> newCols;

                if (newRows > 0 && newCols > 0) {
                    maxDisplayRows = newRows;
                    maxDisplayCols = newCols;

                    terminal.clearScreen();  // Clear the screen for a fresh display
                    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Redisplay the spreadsheet
                } else {
                    cout << "Invalid dimensions entered!";
                }

                break;
            }
            case 'U': row = (row > 1) ? row - 1 : row; break;  // Move cursor up
            case 'D': row = (row < maxDisplayRows) ? row + 1 : row; break;  // Move cursor down
            case 'R': col = (col < maxDisplayCols) ? col + 1 : col; break;  // Move cursor right
            case 'L': col = (col > 1) ? col - 1 : col; break;  // Move cursor left
            case 'q': return 0;  // Quit program if 'q' is pressed

            case 'E': {  // Enter cell data if 'E' is pressed
                terminal.moveCursor(1, 1);  // Move to the space above the table
                cout << sheet.getCellReference(row, col) << ": "; // Show cell reference
                terminal.moveCursor(2, 1);  // Move cursor to input position
                input.clear();  // Clear the input string for new input
                char ch;

                // Capture input character by character
                while (true) {
                    ch = cin.get();  // Read one character at a time
                    if (ch == '\n') {  // If Enter is pressed, stop input
                        break;
                    }

                    cout << ch;  // Echo the character as it is typed
                    input += ch;  // Add the character to the input string
                }

                // After input is done, process the input
                sheet.enterCellData(row, col, input);  // Assuming `enterCellData` is defined in `Spreadsheet` class
                sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Refresh display
                break;
            }

            case 's': {  // Save the spreadsheet to a file if 's' is pressed
                terminal.moveCursor(1, 1);  // Move the cursor to a specific location on the screen
                cout << "Enter filename to save the spreadsheet: ";
                string saveFilename;
                char c;

                // Capture filename character by character
                while (true) {
                    c = std::cin.get();  // Read one character at a time

                    if (c == '\n') {  // Stop when Enter is pressed
                        break;
                    }

                    cout << c;  // Display the character immediately
                    saveFilename += c;  // Append the character to the filename
                }

                // Save the spreadsheet to the specified file
                FileManager::saveToCSV(sheet, saveFilename);
                break;
            }

           case 'l': {  // Load a spreadsheet from a file if 'l' is pressed
                terminal.moveCursor(1, 1);  // Move the cursor to a specific location on the screen
                cout << "Enter filename to load the spreadsheet: ";
                string loadFilename;
                char c;

                // Capture filename character by character
                while (true) {
                    c = std::cin.get();  // Read one character at a time

                    if (c == '\n') {  // Stop when Enter is pressed
                        break;
                    }

                    cout << c;  // Display the character immediately
                    loadFilename += c;  // Append the character to the filename
                }

                // Load the spreadsheet from the specified file
                FileManager::loadFromCSV(sheet, loadFilename);

                // Immediately update and display the spreadsheet
                terminal.clearScreen();  // Clear the screen to redraw the table
                sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);

                break;
            }
            // You can add additional case statements here as needed
        }
    }

    return 0;  // End the program
}
