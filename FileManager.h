#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Spreadsheet.h"
#include <string>

namespace Spreadsheet{
     // The FileManager class handles saving data to and loading data from CSV files for a Spreadsheet.
    class FileManager {
        public:
        // Saves the Spreadsheet object to a CSV file.
        static void saveToCSV(const Spreadsheet& spreadsheet, const std::string& filename);

        // Loads data from a CSV file into the Spreadsheet object.
        static void loadFromCSV(Spreadsheet& spreadsheet, const std::string& filename);
    };
}// namespace Spreadsheet
#endif // FILEMANAGER_H