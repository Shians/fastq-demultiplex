#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

std::vector<std::string> read_barcodes(std::string bc_file_name) {
    std::ifstream barcode_file(bc_file_name);

    std::vector<std::string> barcodes;

    std::string line;
    while (barcode_file >> line) {
        std::stringstream ss(line);

        std::string name, barcode;
        getline(ss, name, ',');
        getline(ss, barcode);

        barcodes.push_back(barcode);
    }

    return barcodes;
}
