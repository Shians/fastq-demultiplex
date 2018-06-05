#include "gzip_wrappers.h"

using std::string;

gzFile gzip_open(const string &filename, const string &mode) {
    return gzopen(filename.c_str(), mode.c_str());
}

void gzip_close(gzFile &file, const string &filename) {
    gzclose(file);
};

/* GzipOutput */
GzipOutput::GzipOutput(const string &filename) {
    _filename = filename;
    _fp = gzip_open(filename, "w");
}

void GzipOutput::close() {
    this->flush();
    gzip_close(_fp, _filename);
}

void GzipOutput::write(const string &s) {
    _write_queue.push(s);
    if (_write_queue.size() > 128) {
        this->flush();
    }
}

void GzipOutput::flush() {
    std::stringstream buf;
    while (_write_queue.size() > 0) {
        buf << _write_queue.front();
        _write_queue.pop();
    }
    if (buf.str() != "") {
        gzputs(_fp, buf.str().c_str());
    }
}

/* OutputPairs */
OutputPairs::OutputPairs(std::vector<string> &barcodes, string &outdir) {
    _keys = barcodes;
    _outdir = outdir;

    for (int i = 0; i < _keys.size(); i++) {
        string r1_name = make_r1_name(_keys[i]);
        string r2_name = make_r2_name(_keys[i]);

        _files1.push_back(GzipOutput(r1_name));
        _files2.push_back(GzipOutput(r2_name));
    }
}

void OutputPairs::close_all() {
    for (int i = 0; i < _keys.size(); i++) {
        _files1[i].close();
        _files2[i].close();
    }
}

void OutputPairs::write_file1(const string &barcode, const string &s) {
    auto it = find(_keys.begin(), _keys.end(), barcode);
    ptrdiff_t i = it - _keys.begin();

    _files1[i].write(s);
}

void OutputPairs::write_file2(const string &barcode, const string &s) {
    auto it = find(_keys.begin(), _keys.end(), barcode);
    ptrdiff_t i = it - _keys.begin();

    _files2[i].write(s);
}

