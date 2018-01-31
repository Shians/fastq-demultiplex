#pragma once
#include <iostream>
#include <cstddef>
#include <zlib.h>
#include <string>
#include <queue>
#include <algorithm>

gzFile gzip_open(std::string filename, std::string mode);
void gzip_close(gzFile file);

class GzipFile {
    public:
        GzipFile(std::string filename);

        void close();
        void write(std::string s);
    private:
        gzFile _fp;
        std::string _filename;
};

class OutputPairs {
    public:
        OutputPairs(std::vector<std::string> barcodes, std::string outdir);

        void close_all();
        void write_file1(std::string barcode, std::string s);
        void write_file2(std::string barcode, std::string s);

    private:
        std::vector<GzipFile> _files1;
        std::vector<GzipFile> _files2;
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
