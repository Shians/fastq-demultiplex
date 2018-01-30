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
        std::queue<std::string> _print_queue;
};

class GzipFiles {
    public:
        GzipFiles(std::vector<std::string> barcodes, std::string outdir);
        ~GzipFiles();

        GzipFile get_file1(std::string barcode);
        GzipFile get_file2(std::string barcode);

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
