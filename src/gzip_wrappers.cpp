#include "gzip_wrappers.h"

gzFile gzip_open(std::string filename, std::string mode) {
    return gzopen(filename.c_str(), mode.c_str());
}

void gzip_close(gzFile file, std::string filename) {
    gzclose(file);
};

GzipFile::GzipFile(std::string filename) {
    _filename = filename;
    _fp = gzip_open(filename, "w");
}

void GzipFile::close() {
    std::string buf = "";
    if (_print_queue.size() > 0) {
        while (_print_queue.size() > 0) {
            buf += _print_queue.front();
            _print_queue.pop();
        }
        gzprintf(_fp, "%s", buf.c_str());
    }
    gzip_close(_fp, _filename);
}

void GzipFile::write(std::string s) {
    _print_queue.push(s);
    if (_print_queue.size() > 256) {
        std::string buf = "";
        while (_print_queue.size() > 0) {
            buf += _print_queue.front();
            _print_queue.pop();
        }
        gzprintf(_fp, "%s", buf.c_str());
    }
}

GzipFiles::GzipFiles(std::vector<std::string> barcodes, std::string outdir) {
    _keys = barcodes;
    _outdir = outdir;

    for (int i = 0; i < _keys.size(); i++) {
        std::string r1_name = make_r1_name(_keys[i]);
        std::string r2_name = make_r2_name(_keys[i]);

        _files1.push_back(GzipFile(r1_name));
        _files2.push_back(GzipFile(r2_name));
    }
}

GzipFiles::~GzipFiles() {
    for (int i = 0; i < _keys.size(); i++) {
        _files1[i].close();
        _files2[i].close();
    }
}

GzipFile GzipFiles::get_file1(std::string barcode) {
    auto it = find(_keys.begin(), _keys.end(), barcode);
    ptrdiff_t i = it - _keys.begin();

    return _files1[i];
}

GzipFile GzipFiles::get_file2(std::string barcode) {
    auto it = find(_keys.begin(), _keys.end(), barcode);
    ptrdiff_t i = it - _keys.begin();

    return _files2[i];
}

