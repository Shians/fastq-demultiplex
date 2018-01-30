#pragma once
#include <cstddef>
#include <zlib.h>
#include <string>
#include <queue>

#include "options.h"

class GzipFile {
    public:
        GzipFile(std::string filename) {
            _filename = filename;
            _fp = gzopen(filename.c_str(), "w");
            gzclose(_fp);
        }

        void flush() {
            _fp = gzopen(_filename.c_str(), "a");
            std::string buf = "";
            while (_print_queue.size() > 0) {
                buf += _print_queue.front() + "\n";
                _print_queue.pop();
            }
            gzprintf(_fp, "%s", buf.c_str());
            gzclose(_fp);
        }

        void write(std::string s) {
            if (_print_queue.size() < 256) {
                _print_queue.push(s);
            } else {
                this->flush();
            }
        }
    private:
        gzFile _fp;
        std::string _filename;
        std::queue<std::string> _print_queue;
};

class GzipFiles {
    public:
        GzipFiles(std::vector<std::string> barcodes) {
            _keys = barcodes;

            for (int i = 0; i < _keys.size(); i++) {
                std::string r1_name = make_r1_name(_keys[i]);
                std::string r2_name = make_r2_name(_keys[i]);

                _files1.push_back(GzipFile(r1_name));
                _files2.push_back(GzipFile(r2_name));
            }
        }

        GzipFile get_file1(std::string barcode) {
            auto it = find(_keys.begin(), _keys.end(), barcode);
            ptrdiff_t i = it - _keys.begin();

            return _files1[i];
        }

        GzipFile get_file2(std::string barcode) {
            auto it = find(_keys.begin(), _keys.end(), barcode);
            ptrdiff_t i = it - _keys.begin();

            return _files2[i];
        }

    private:
        std::vector<GzipFile> _files1;
        std::vector<GzipFile> _files2;
        std::vector<std::string> _keys;

        std::string make_filename(std::string bc, std::string read) {
            return PRG_OPTS.outdir + "/" + bc + "_R" + read + ".fastq.gz";
        }

        std::string make_r1_name(std::string bc) {
            return make_filename(bc, "1");
        }

        std::string make_r2_name(std::string bc) {
            return make_filename(bc, "2");
        }
};
