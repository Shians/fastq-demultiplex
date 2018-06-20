#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <experimental/filesystem>

#include "options.hpp"
#include "fastq.hpp"
#include "gzip_wrappers.hpp"
#include "fmt/format.h"

void demultiplex(std::vector<std::string> const &barcodes);
