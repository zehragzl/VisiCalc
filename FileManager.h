#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Spreadsheet.h"
#include <string>

namespace Spreadsheet{
    class FileManager {
        public:
        static void saveToCSV(const Spreadsheet& spreadsheet, const std::string& filename);

        static void loadFromCSV(Spreadsheet& spreadsheet, const std::string& filename);
    };
}
#endif