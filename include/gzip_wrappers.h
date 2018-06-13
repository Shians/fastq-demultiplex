#pragma once
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <experimental/filesystem>

#include <iostream>
#include <sstream>
#include <cstddef>
#include <zlib.h>
#include <string>
#include <queue>
#include <algorithm>

gzFile gzip_open(const std::string &filename, const std::string &mode);
void gzip_close(gzFile file);

struct GzipOutput {
    GzipOutput(const std::string &filename, const size_t compression_level);
    GzipOutput(const std::string &filename) : GzipOutput(filename, 4) {};

    void close();
    void write(const std::string &s);
    void flush();
private:
    gzFile fp_;
    std::string filename_;
    std::queue<std::string> write_queue_;
};

struct OutputPairs {
    OutputPairs(std::vector<std::string> &barcodes, std::string &outdir);

    void close_all();
    void write_file1(const std::string &barcode, const std::string &s);
    void write_file2(const std::string &barcode, const std::string &s);

private:
    std::vector<GzipOutput> files1_;
    std::vector<GzipOutput> files2_;
    std::vector<std::string> keys_;
    std::string outdir_;

    std::string make_filename(std::string bc, std::string read) {
        return outdir_ + "/" + bc + "_R" + read + ".fastq.gz";
    }

    std::string make_r1_name(std::string bc) {
        return make_filename(bc, "1");
    }

    std::string make_r2_name(std::string bc) {
        return make_filename(bc, "2");
    }
};
