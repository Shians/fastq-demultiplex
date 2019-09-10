#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "fmt/format.h"

std::vector<std::string> read_barcodes(std::string bc_file_name, int length) {
    if (length <= 0) {
        throw std::runtime_error("invalid start and end positions");
    }
    std::ifstream barcode_file(bc_file_name);

    std::vector<std::string> barcodes;

    std::string line;
    while (barcode_file >> line) {
        std::stringstream ss(line);

        std::string name, barcode;
        getline(ss, name, ',');
        getline(ss, barcode);

        if (barcode.length() != length) {
            throw std::runtime_error(fmt::format("invalid barcode: {}", barcode));
        }
        barcodes.push_back(barcode);
    }

    return barcodes;
}
