#include <filesystem>
#include <string>
#include <iostream>

#include "file_utility.h"

FileUtility::FileUtility() {

}

FileUtility::~FileUtility() {

}

int FileUtility::WriteDirectory(const std::string filePath) {

    namespace fs = std::filesystem;
/*
        try {

            // If exists, assume success
            if (std::filesystem::exists(filePath)) {
                return 0;
            }

            if (std::filesystem::create_directory(filePath))
                std::cout << "Created a directory -> " << filePath.c_str() << "\n";
            else
                std::cerr << "Failed to create a directory\n";
            return -1;
        } catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
*/
    return 0;
}


