#include "gzip_wrappers.hpp"

using std::string;

// open gzip file
gzFile gzip_open(string const &filename, string const &mode) {
    return gzopen(filename.c_str(), mode.c_str());
}

// close gzip file
void gzip_close(gzFile &file, string const &filename) {
    gzclose(file);
};

/* GzipOutput */
GzipOutput::GzipOutput(string const &filename, size_t const compression_level) {
    size_t comp_level = compression_level;
    comp_level = std::min(comp_level, static_cast<unsigned long>(9));
    comp_level = std::max(comp_level, static_cast<unsigned long>(1));

    string fmode = fmt::format("{}{}", filename, comp_level);
    filename_ = filename;
    fp_ = gzip_open(filename, fmode);
}

void GzipOutput::close() {
    this->flush();
    gzip_close(fp_, filename_);
}

void GzipOutput::write(string const &s) {
    write_queue_.push(s);
    if (write_queue_.size() > 128) {
        this->flush();
    }
}

void GzipOutput::flush() {
    std::stringstream buf;
    while (write_queue_.size() > 0) {
        buf << write_queue_.front();
        write_queue_.pop();
    }
    if (buf.str() != "") {
        gzputs(fp_, buf.str().c_str());
    }
}

/* OutputPairs */
OutputPairs::OutputPairs(std::vector<string> const &barcodes, string const &outdir) {
    keys_ = barcodes;
    outdir_ = outdir;

    for (int i = 0; i < keys_.size(); i++) {
        string r1_name = make_r1_name(keys_[i]);
        string r2_name = make_r2_name(keys_[i]);

        files1_.push_back(std::make_unique<GzipOutput>(r1_name));
        files2_.push_back(std::make_unique<GzipOutput>(r2_name));
    }
}

void OutputPairs::close_all() {
    for (int i = 0; i < keys_.size(); i++) {
        files1_[i]->close();
        files2_[i]->close();
    }
}

void OutputPairs::write_file1(string const &barcode, string const &s) {
    auto bc_ind = find(keys_.begin(), keys_.end(), barcode);
    auto i = std::distance(keys_.begin(), bc_ind);

    files1_[i]->write(s);
}

void OutputPairs::write_file2(string const &barcode, string const &s) {
    auto bc_ind = find(keys_.begin(), keys_.end(), barcode);
    auto i = std::distance(keys_.begin(), bc_ind);

    files2_[i]->write(s);
}
