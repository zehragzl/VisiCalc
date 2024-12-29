#include "AnsiTerminal.h"
#include "Spreadsheet.h"
#include "FileManager.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;
using namespace Spreadsheet;

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clear the input buffer
}

int getPositiveInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear();
            clearInputBuffer();
            cout << "Invalid input. Please enter a positive integer." << endl;
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

string getInputString(const string& prompt) {
    cout << prompt;
    string input;
    char ch;
    while (true) {
        ch = cin.get();  // Read one character at a time
        if (ch == '\n') {  // Stop when Enter is pressed
            break;
        }
        if (ch == 127) {  // Handle backspace (ASCII 127)
            if (!input.empty()) {
                input.pop_back();  // Remove last character
                cout << "\b \b";  // Move cursor back and overwrite the character with space
            }
        } else {
            input += ch;  // Add the character to the input string
            cout << ch;  // Echo the character as it is typed
        }
    }
    return input;
}

int main() {
    AnsiTerminal terminal;  // Create an instance of the terminal class
    terminal.clearScreen(); // Clear the screen at the beginning

    Spreadsheet::Spreadsheet sheet(100, 100); // Create a spreadsheet with 100 rows and 100 columns
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
                maxDisplayRows = getPositiveInt("Enter new number of display rows: ");
                maxDisplayCols = getPositiveInt("Enter new number of display columns: ");

                terminal.clearScreen();  // Clear the screen for a fresh display
                sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Redisplay the spreadsheet
                break;
            }
            case 'U': row = max(1, row - 1); break;  // Move cursor up
            case 'D': row = min(maxDisplayRows, row + 1); break;  // Move cursor down
            case 'R': col = min(maxDisplayCols, col + 1); break;  // Move cursor right
            case 'L': col = max(1, col - 1); break;  // Move cursor left
            case 'q': return 0;  // Quit program if 'q' is pressed

            case 'E': {  // Enter cell data if 'E' is pressed
                terminal.moveCursor(1, 1);  // Move to the space above the table
                cout << sheet.getCellReference(row, col) << ": "; // Show cell reference
                terminal.moveCursor(2, 1);  // Move cursor to input position
                input.clear();  // Clear the input string for new input

                // Capture input character by character
                char ch;
                while (true) {
                    ch = cin.get();  // Read one character at a time
                    if (ch == '\n') {  // If Enter is pressed, stop input
                        break;
                    }

                    cout << ch;  // Echo the character as it is typed
                    input += ch;  // Add the character to the input string
                }

                // After input is done, process the input
                try {
                    sheet.enterCellData(row, col, input);  // Assuming `enterCellData` is defined in `Spreadsheet` class
                    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Refresh display
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;  // Display any error that occurs
                }
                break;
            }

            case 's': {  // Save the spreadsheet to a file if 's' is pressed
                terminal.moveCursor(1, 1); 
                string saveFilename = getInputString("Enter filename to save the spreadsheet: ");
                try {
                    FileManager::saveToCSV(sheet, saveFilename);
                    cout << "Spreadsheet saved successfully." << endl;
                } catch (const exception& e) {
                    cout << "Error saving file: " << e.what() << endl;
                }
                break;
            }

           case 'l': {  // Load a spreadsheet from a file if 'l' is pressed
                terminal.moveCursor(1, 1); 
                string loadFilename = getInputString("Enter filename to load the spreadsheet: ");
                try {
                    FileManager::loadFromCSV(sheet, loadFilename);
                    terminal.clearScreen();  // Clear the screen to redraw the table
                    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);
                    cout << "Spreadsheet loaded successfully." << endl;
                } catch (const exception& e) {
                    cout << "Error loading file: " << e.what() << endl;
                }
                break;
            }
            // You can add additional case statements here as needed
        }
    }

    return 0;  // End the program
}