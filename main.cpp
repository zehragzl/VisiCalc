#include "AnsiTerminal.h"
#include "Spreadsheet.h"
#include "FileManager.h"
#include <iostream>
#include <string>
#include <limits>

using namespace std;
using namespace Spreadsheet;

// Function to clear the input buffer
void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Clears the input buffer to prevent invalid input
}

// Function to get a positive integer from the user with input validation
int getPositiveInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;  // Display the prompt to the user
        cin >> value;  // Read user input
        if (cin.fail() || value <= 0) {  // Check for invalid input or non-positive integers
            cin.clear();  // Clear the error flag on cin
            clearInputBuffer();  // Clear the input buffer
            cout << "Invalid input. Please enter a positive integer." << endl;
        } else {
            clearInputBuffer();  // Clear the buffer before returning value
            return value;
        }
    }
}

// Function to get a string input from the user (handles backspace and displays typed characters)
string getInputString(const string& prompt) {
    cout << prompt;  // Display the prompt
    string input;  // String to store the input
    char ch;
    while (true) {
        ch = cin.get();  // Read one character at a time
        if (ch == '\n') {  // Stop when Enter is pressed
            break;
        }
        if (ch == 127) {  // Handle backspace (ASCII 127)
            if (!input.empty()) {
                input.pop_back();  // Remove last character from the string
                cout << "\b \b";  // Move cursor back and overwrite the character with space
            }
        } else {
            input += ch;  // Append character to the input string
            cout << ch;  // Echo the character typed by the user
        }
    }
    return input;  // Return the final input string
}

int main() {
    AnsiTerminal terminal;  // Create an instance of the terminal class for managing terminal output
    terminal.clearScreen();  // Clear the terminal screen at the start of the program

    Spreadsheet::Spreadsheet sheet(100, 100); // Create a spreadsheet object with 100 rows and 100 columns
    int row = 1, col = 1;  // Initial cursor position in the spreadsheet
    int maxDisplayRows = 20;  // Maximum rows to display at once
    int maxDisplayCols = 80;  // Maximum columns to display at once

    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols); // Initial display of the spreadsheet

    char key;  // Variable to hold the key pressed by the user
    string input;  // String to hold input for cell data

    // Main loop to handle user input and actions
    while (true) {
        terminal.moveCursor(row + 3, col * 10);  // Move cursor to the current cell's position
        key = terminal.getSpecialKey();  // Get key input from the user

        switch (key) {
            case 'G': {  // Case for adjusting display dimensions
                terminal.moveCursor(1, 1);  // Move cursor to the top-left corner
                maxDisplayRows = getPositiveInt("Enter new number of display rows: ");  // Get new number of rows
                maxDisplayCols = getPositiveInt("Enter new number of display columns: ");  // Get new number of columns

                terminal.clearScreen();  // Clear the screen for fresh display
                sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Redisplay the spreadsheet with new dimensions
                break;
            }
            case 'U': row = max(1, row - 1); break;  // Move cursor up
            case 'D': row = min(maxDisplayRows, row + 1); break;  // Move cursor down
            case 'R': col = min(maxDisplayCols, col + 1); break;  // Move cursor right
            case 'L': col = max(1, col - 1); break;  // Move cursor left
            case 'q': return 0;  // Quit the program if 'q' is pressed

            case 'E': {  // Case for entering cell data
                terminal.moveCursor(1, 1);  // Move to the space above the table
                cout << sheet.getCellReference(row, col) << ": "; // Show the cell reference
                terminal.moveCursor(2, 1);  // Move cursor to input position
                input.clear();  // Clear the input string for new input

                // Capture input character by character
                char ch;
                while (true) {
                    ch = cin.get();  // Read one character at a time
                    if (ch == '\n') {  // If Enter is pressed, stop input
                        break;
                    }

                    cout << ch;  // Echo the character typed by the user
                    input += ch;  // Append the character to the input string
                }

                // After input is done, process the input
                try {
                    sheet.enterCellData(row, col, input);  // `enterCellData` is a method in the Spreadsheet class
                    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Refresh display after input
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;  // Handle any errors that occur during input processing
                }
                break;
            }

            case 's': {  // Case for saving the spreadsheet to a file (press 's' to save)
                terminal.moveCursor(1, 1); 
                string saveFilename = getInputString("Enter filename to save the spreadsheet: ");  // Get filename from user
                try {
                    FileManager::saveToCSV(sheet, saveFilename);  // Save spreadsheet data to CSV file
                    cout << "Spreadsheet saved successfully." << endl;
                } catch (const exception& e) {
                    cout << "Error saving file: " << e.what() << endl;  // Handle any errors during file saving
                }
                break;
            }

            case 'l': {  // Case for loading a spreadsheet from a file (press 'l' to load)
                terminal.moveCursor(1, 1); 
                string loadFilename = getInputString("Enter filename to load the spreadsheet: ");  // Get filename from user
                try {
                    FileManager::loadFromCSV(sheet, loadFilename);  // Load spreadsheet data from CSV file
                    terminal.clearScreen();  // Clear the screen to redraw the table
                    sheet.displaySpreadsheet(maxDisplayRows, maxDisplayCols);  // Redisplay the spreadsheet
                    cout << "Spreadsheet loaded successfully." << endl;
                } catch (const exception& e) {
                    cout << "Error loading file: " << e.what() << endl;  // Handle any errors during file loading
                }
                break;
            }
           
        }
    }

    return 0;  // End the program
}
