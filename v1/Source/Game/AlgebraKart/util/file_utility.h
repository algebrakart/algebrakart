#pragma once

#include <filesystem>
#include <string>
#include <iostream>


class FileUtility {
private:

public:
    FileUtility();
    ~FileUtility();
    static int WriteDirectory(const std::string filePath);
};