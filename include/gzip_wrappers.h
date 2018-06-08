#pragma once
#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <iostream>
#include <sstream>
#include <cstddef>
#include <zlib.h>
#include <string>
#include <queue>
#include <algorithm>

gzFile gzip_open(const std::string &filename, const std::string &mode);
void gzip_close(gzFile file);

class GzipOutput {
    public:
        GzipOutput(const std::string &filename, const size_t compression_level);
        GzipOutput(const std::string &filename) : GzipOutput(filename, 4) {};

        void close();
        void write(const std::string &s);
        void flush();
    private:
        gzFile _fp;
        std::string _filename;
        std::queue<std::string> _write_queue;
};

class OutputPairs {
    public:
        OutputPairs(std::vector<std::string> &barcodes, std::string &outdir);

        void close_all();
        void write_file1(const std::string &barcode, const std::string &s);
        void write_file2(const std::string &barcode, const std::string &s);

    private:
        std::vector<GzipOutput> _files1;
        std::vector<GzipOutput> _files2;
        std::vector<std::string> _keys;
        std::string _outdir;

        std::string make_filename(std::string bc, std::string read) {
            return _outdir + "/" + bc + "_R" + read + ".fastq.gz";
        }

        std::string make_r1_name(std::string bc) {
            return make_filename(bc, "1");
        }

        std::string make_r2_name(std::string bc) {
            return make_filename(bc, "2");
        }
};
