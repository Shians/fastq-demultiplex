#include "demultiplex.hpp"

using std::string;
using std::vector;

extern options_t PRG_OPTS;

typedef std::experimental::filesystem::path FilePath;
typedef std::pair<FastqRecord, FastqRecord> FastqRecordPair;

static unsigned long records_processed = 0;

void process_record_pairs(
    vector<FastqRecordPair> &record_queue,
    OutputPairs &files,
    GzipOutput &undetermined1,
    GzipOutput &undetermined2,
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

        if (binary_search(barcodes.begin(), barcodes.end(), barcode)) {
            // if barcode matches write to named file
            files.write_file1(barcode, record1.str());
            files.write_file2(barcode, record2.str());
        } else {
            // else write to undetermined file
            undetermined1.write(record1.str());
            undetermined2.write(record2.str());
        }

        records_processed++;
    }
    record_queue.clear();
}

void demultiplex(vector<string> const &barcodes) {
    string r1 = PRG_OPTS.r1;
    string r2 = PRG_OPTS.r2;
    string outdir = PRG_OPTS.outdir;
    
    OutputPairs files(barcodes, outdir);

    FilePath out_path_r1 = outdir;
    out_path_r1 /= "Undetermined_R1.fastq.gz";
    GzipOutput undetermined1(out_path_r1.string());

    FilePath out_path_r2 = outdir;
    out_path_r2 /= "Undetermined_R2.fastq.gz";
    GzipOutput undetermined2(out_path_r2.string());

    FastqFile fq1(r1);
    FastqFile fq2(r2);

    FastqRecord record1;
    FastqRecord record2;

    vector<FastqRecordPair> record_queue;

    do {
        for (int i = 0; i < 512; i++) {
            record1 = fq1.get_record();
            record2 = fq2.get_record();
            if (!record1.good || !record2.good) {
                break;
            }

            record_queue.push_back(std::make_pair(record1, record2));
        }

        process_record_pairs(record_queue, files, undetermined1, undetermined2, barcodes);

        if (records_processed % 500000 == 0) {
            std::cout << records_processed << " records processed..." << std::endl;
        }
    } while (record1.good && record2.good);

    files.close_all();
    undetermined1.close();
    undetermined2.close();

    std::cout << records_processed << " total records processed" << "\n";
}
