#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>
#include "Spreadsheet.h"

// The FileManager class handles loading and saving spreadsheet data to CSV files.
class FileManager {
public:

    // Saves the spreadsheet data to a CSV file with the given filename
    static void saveToCSV(const Spreadsheet &spreadsheet, const std::string &filename);

    // Loads the spreadsheet data from a CSV file with the given filename
    static void loadFromCSV(Spreadsheet &spreadsheet, const std::string &filename);
};

#endif