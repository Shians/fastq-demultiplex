#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>

#include "options.hpp"
#include "fastq.hpp"
#include "gzip_wrappers.hpp"

void demultiplex(std::vector<std::string> const &barcodes);
