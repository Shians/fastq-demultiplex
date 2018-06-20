#include "demultiplex.hpp"

using std::string;
using std::vector;

extern options_t PRG_OPTS;

typedef std::pair<FastqRecord, FastqRecord> FastqRecordPair;

static unsigned long records_processed = 0;

void process_record_pairs(
    vector<FastqRecordPair> &record_queue,
    OutputPairs &files,
    vector<string> const &barcodes
) {
    if (record_queue.size() == 0) {
        return;
    }

    int bc_start = PRG_OPTS.bc_start;
    int bc_end = PRG_OPTS.bc_end;
    bool r2_bc = PRG_OPTS.r2_bc;

    for (auto const &pair : record_queue) {
        string barcode;

        auto const record1 = pair.first;
        auto const record2 = pair.second;

        if (!r2_bc) {
            barcode = record1.seq_substr(bc_start, bc_end);
        } else {
            barcode = record2.seq_substr(bc_start, bc_end);
        }

        files.write_files(barcode, record1.str(), record2.str());

        records_processed++;
        if (records_processed % 500000 == 0) {
            std::cout.imbue(std::locale(""));
            std::cout << records_processed << " records processed..." << std::endl;
        }
    }
    record_queue.clear();
}

void demultiplex(vector<string> const &barcodes) {
    string r1 = PRG_OPTS.r1;
    string r2 = PRG_OPTS.r2;
    string outdir = PRG_OPTS.outdir;
    
    OutputPairs files(barcodes, outdir);

    FastqFile fq1(r1);
    FastqFile fq2(r2);

    FastqRecord record1;
    FastqRecord record2;

    size_t const BATCH_SIZE = 512;
    vector<FastqRecordPair> record_queue;
    record_queue.reserve(BATCH_SIZE);

    do {
        for (int i = 0; i < BATCH_SIZE; i++) {
            record1 = fq1.get_record();
            record2 = fq2.get_record();
            if (!record1.good || !record2.good) {
                break;
            }

            record_queue.push_back(std::make_pair(record1, record2));
        }

        process_record_pairs(record_queue, files, barcodes);

    } while (record1.good && record2.good);

    files.close_all();

    std::cout.imbue(std::locale(""));
    std::cout << records_processed << " records processed..." << "\n";
}
