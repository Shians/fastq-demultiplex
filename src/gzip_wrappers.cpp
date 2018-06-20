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

    string fmode = fmt::format("wb{}", comp_level);
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

        typedef boost::filesystem::path FilePath;

        FilePath out_path_r1 = outdir;
        out_path_r1 /= "Undetermined_R1.fastq.gz";
        undetermined1_ = std::make_unique<GzipOutput>(out_path_r1.string());

        FilePath out_path_r2 = outdir;
        out_path_r2 /= "Undetermined_R2.fastq.gz";
        undetermined2_ = std::make_unique<GzipOutput>(out_path_r2.string());
    }
}

void OutputPairs::close_all() {
    for (int i = 0; i < keys_.size(); i++) {
        files1_[i]->close();
        files2_[i]->close();
    }
    undetermined1_->close();
    undetermined2_->close();
}

int hamming_dist(string const &s1, string const &s2) {
    if (s1.size() != s2.size()) {
        throw std::runtime_error("string lengths do not match");
    }

    int dist = 0;

    for (int i = 0; i < s1.size(); i++) {
        dist += s1[i] != s2[i];
    }

    return dist;
}

// find the index of the barcode with closest match
// returns -1 if maximum mismatch is exceeded or multiple barcodes are within
// mismatch limit
int OutputPairs::get_closest_match(std::string const &barcode) {
    int max_mismatch = PRG_OPTS.mismatch;

    // if exact match can be found then 
    auto exact_match = std::lower_bound(keys_.begin(), keys_.end(), barcode);
    if (exact_match != keys_.end()) {
        return std::distance(keys_.begin(), exact_match);
    }

    // if no exact match is found and no mismatch is allowed then can return -1 here
    if (max_mismatch == 0) {
        return -1;
    }

    std::vector<int> ham_dist_vec;
    ham_dist_vec.reserve(keys_.size());

    // check hamming distance to every annotated barcode
    std::transform(
        std::begin(keys_),
        std::end(keys_),
        std::back_inserter(ham_dist_vec),
        [&] (auto const &key) { return hamming_dist(barcode, key); }
    );

    // find smallest hamming distance
    // if smallest hamming distance is greater than mismatch return -1
    int min_ham_dist = std::accumulate(
        std::begin(ham_dist_vec),
        std::end(ham_dist_vec),
        std::numeric_limits<int>::max(),
        [] (int a, int b) { return std::min(a, b); }
    );
    if (min_ham_dist > max_mismatch) {
        return -1;
    }

    // if multiple barcodes are tied at smallest hamming distance then return -1
    int dists_at_min = std::count_if(
        std::begin(ham_dist_vec),
        std::end(ham_dist_vec),
        [min_ham_dist] (int val) { return val == min_ham_dist; }
    );
    if (dists_at_min > 1) {
        return -1;
    }

    auto ham_match = std::find(std::begin(ham_dist_vec), std::end(ham_dist_vec), min_ham_dist);
    return std::distance(ham_dist_vec.begin(), ham_match);
}

void OutputPairs::write_files(string const &barcode, string const &s1, string const &s2) {
    auto ind = get_closest_match(barcode);
    write_file1(barcode, s1, ind);
    write_file2(barcode, s2, ind);
}

void OutputPairs::write_file1(string const &barcode, string const &s, int ind) {
    if (ind < 0) {
        undetermined1_->write(s);
    } else {
        files1_[ind]->write(s);
    }
}

void OutputPairs::write_file2(string const &barcode, string const &s, int ind) {
    if (ind < 0) {
        undetermined2_->write(s);
    } else {
        files2_[ind]->write(s);
    }
}
