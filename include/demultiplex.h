#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "options.h"
#include "fastq.h"
#include "gzip_wrappers.h"

void demultiplex(std::vector<std::string> const &barcodes);
