#pragma once
#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "options.hpp"

#include <experimental/filesystem>
#include <zlib.h>

#include <iostream>
#include <sstream>
#include <cstddef>
#include <string>
#include <numeric>
#include <limits>
#include <queue>
#include <mutex>
#include <algorithm>

gzFile gzip_open(std::string const &filename, std::string const &mode);
void gzip_close(gzFile file);

// class to represent a Gzip output file
struct GzipOutput {
    GzipOutput(std::string const &filename, size_t const compression_level);
    GzipOutput(std::string const &filename) : GzipOutput(filename, 4) {};

    void close();
    void write(std::string const &s);
    void flush();

private:
    gzFile fp_;
    std::string filename_;
    std::queue<std::string> write_queue_;
    // std::mutex mutex_;
};

// class to represent a pair of Gzip output files
struct OutputPairs {
    OutputPairs(std::vector<std::string> const &barcodes, std::string const &outdir);

    void close_all();
    int get_closest_match(std::string const &barcode);
    void write_files(std::string const &barcode, std::string const &s1, std::string const &s2);
    void write_file1(std::string const &barcode, std::string const &s, int ind);
    void write_file2(std::string const &barcode, std::string const &s, int ind);

private:
    std::vector<std::unique_ptr<GzipOutput>> files1_;
    std::vector<std::unique_ptr<GzipOutput>> files2_;
    std::unique_ptr<GzipOutput> undetermined1_;
    std::unique_ptr<GzipOutput> undetermined2_;
    std::vector<std::string> keys_;
    std::string outdir_;

    typedef std::experimental::filesystem::path file_path;
    std::string make_filename(std::string const &bc, std::string const &read) {
        file_path out_path = outdir_;
        out_path /=  bc + "_R" + read + ".fastq.gz";
        return out_path.string();
    }

    std::string make_r1_name(std::string const &bc) {
        return make_filename(bc, "1");
    }

    std::string make_r2_name(std::string const &bc) {
        return make_filename(bc, "2");
    }
};
