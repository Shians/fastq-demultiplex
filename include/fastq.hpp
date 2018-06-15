#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <zlib.h>
#include <kseq.h>

KSEQ_INIT(gzFile, gzread);

// class representation of a single fastq record
class FastqRecord {
    public:
        std::string name;
        std::string comment;
        std::string seq;
        std::string qual;
        bool good;

        friend std::ostream& operator<<(std::ostream &os, const FastqRecord &obj) {
            os << "@" << obj.name << " " << obj.comment << "\n"
                << obj.seq << "\n"
                << "+" << "\n"
                << obj.qual << "\n";
            return os;
        }

        // get the record as a string
        std::string str() const {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }

        // take a sub-range of the sequence
        std::string seq_substr(int start, int end) const {
            int sub_len = end - start + 1;
            
            // input is 1-indexed, convert to 0-index
            return seq.substr(start-1, sub_len);
        }

        // take a sub-range of the record (sequence and quality)
        FastqRecord substr(int start, int end) const {
            int sub_len = end - start + 1;

            FastqRecord record;
            record.name = name;
            record.comment = comment;

            // input is 1-indexed, convert to 0-index
            record.seq = seq.substr(start-1, sub_len);
            record.qual = qual.substr(start-1, sub_len);

            return record;
        }
};

// class representation of a fastq file
class FastqFile {
    public:
        FastqFile(std::string filename) {
            _filename = filename;
            std::cout << "reading from " << filename << "\n";
            _fp = gzopen(_filename.c_str(), "r");
            _seq = kseq_init(_fp);
        }

        ~FastqFile() {
            kseq_destroy(_seq);
            gzclose(_fp);
        }

        FastqRecord get_record(void) {
            FastqRecord record;
            int l = kseq_read(_seq);

            record.name = _seq->name.s;
            record.comment = _seq->comment.s;
            record.seq = _seq->seq.s;
            record.qual = _seq->qual.s;
            record.good = (l >= 0);

            return record;
        }
    private:
        std::string _filename;
        gzFile _fp;
        kseq_t *_seq;
};
