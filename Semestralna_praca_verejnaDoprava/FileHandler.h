#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// The 'FileHandler' class is a template class that reads data from a file and stores it in a vector of 'Container' type.
template <typename Container>
class FileHandler
{
private:
    std::vector<Container> rowOfData_;
    char delimiter_ = ';'; // Default delimiter is a semicolon

public:
    explicit FileHandler(const char delim = ';') : delimiter_(delim) {}

    void readFromFile(const std::string& fileName);

    // The 'begin' and 'end' functions return iterators to the beginning and end of the 'rowOfData_' vector, respectively.
    typename std::vector<Container>::iterator begin()
    {
        return rowOfData_.begin();
    }

    typename std::vector<Container>::iterator end()
    {
        return rowOfData_.end();
    }
};

template <typename Container>
void FileHandler<Container>::readFromFile(const std::string& fileName) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!");
    }

    std::string line;
    std::getline(file, line); // Skip the first line

    // Read the rest of the lines from the file.
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<std::string> row;
        std::string field;

        // Split each line into fields based on the delimiter and add the fields to the 'row' vector.
        while (std::getline(iss, field, delimiter_)) {
            row.emplace_back(std::move(field));
        }

        // Add the 'row' vector to the 'rowOfData_' vector.
        rowOfData_.emplace_back(row);
    }
}