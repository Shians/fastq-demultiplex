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
};

// class to represent a pair of Gzip output files
struct OutputPairs {
    OutputPairs(std::vector<std::string> &barcodes, std::string &outdir);

    void close_all();
    void write_file1(std::string const &barcode, std::string const &s);
    void write_file2(std::string const &barcode, std::string const &s);

private:
    std::vector<GzipOutput> files1_;
    std::vector<GzipOutput> files2_;
    std::vector<std::string> keys_;
    std::string outdir_;

    typedef std::experimental::filesystem::path file_path;
    std::string make_filename(std::string const &bc, std::string const &read) {
        file_path out_path = outdir_;
        out_path /=  bc + "_R" + read + "fastq.gz";
        return out_path.string();
    }

    std::string make_r1_name(std::string const &bc) {
        return make_filename(bc, "1");
    }

    std::string make_r2_name(std::string const &bc) {
        return make_filename(bc, "2");
    }
};
