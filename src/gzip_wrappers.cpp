#include "gzip_wrappers.h"

using std::string;

gzFile gzip_open(string filename, string mode) {
    return gzopen(filename.c_str(), mode.c_str());
}

void gzip_close(gzFile file, string filename) {
    gzclose(file);
};

GzipFile::GzipFile(string filename) {
    _filename = filename;
    _fp = gzip_open(filename, "w");
}

void GzipFile::close() {
    gzip_close(_fp, _filename);
}

void GzipFile::write(string s) {
    gzprintf(_fp, "%s", s.c_str());
}

GzipFiles::GzipFiles(std::vector<string> barcodes, string outdir) {
    _keys = barcodes;
    _outdir = outdir;

    for (int i = 0; i < _keys.size(); i++) {
        string r1_name = make_r1_name(_keys[i]);
        string r2_name = make_r2_name(_keys[i]);

        _files1.push_back(GzipFile(r1_name));
        _files2.push_back(GzipFile(r2_name));
    }
}

void GzipFiles::close_all() {
    for (int i = 0; i < _keys.size(); i++) {
        _files1[i].close();
        _files2[i].close();
    }
}

void GzipFiles::write_file1(string barcode, string s) {
    auto it = find(_keys.begin(), _keys.end(), barcode);
    ptrdiff_t i = it - _keys.begin();

    _files1[i].write(s);
}

void GzipFiles::write_file2(string barcode, string s) {
    auto it = find(_keys.begin(), _keys.end(), barcode);
    ptrdiff_t i = it - _keys.begin();

    _files2[i].write(s);
}

