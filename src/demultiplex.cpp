#include "demultiplex.h"

using std::string;
using std::vector;

void demultiplex(vector<string> barcodes, options_t PRG_OPTS) {
    string r1 = PRG_OPTS.r1;
    string r2 = PRG_OPTS.r2;
    int bc_start = PRG_OPTS.bc_start;
    int bc_end = PRG_OPTS.bc_end;
    bool r2_bc = PRG_OPTS.r2_bc;
    string outdir = PRG_OPTS.outdir;
    
    GzipFiles files(barcodes, PRG_OPTS.outdir);
    GzipFile undetermined1(outdir + "/Undetermined_R1.fastq.gz");
    GzipFile undetermined2(outdir + "/Undetermined_R2.fastq.gz");

    Fastq_file fq1(r1);
    Fastq_file fq2(r2);

    unsigned long records_processed = 0;

    Fastq_record record1;
    Fastq_record record2;

    do {
        record1 = fq1.get_record();
        record2 = fq2.get_record();

        if (!record1.good || !record2.good) {
            break;
        }

        string barcode;
        if (!r2_bc) {
            barcode = record1.seq_substr(bc_start, bc_end);
        } else {
            barcode = record2.seq_substr(bc_start, bc_end);
        }

        if (binary_search(barcodes.begin(), barcodes.end(), barcode)) {
            files.get_file1(barcode).write(record1.str());
            files.get_file2(barcode).write(record2.str());
        } else {
            undetermined1.write(record1.str());
            undetermined2.write(record2.str());
        }

        records_processed++;
        if (records_processed % 500000 == 0) {
            std::cout << records_processed << " records processed..." << "\n";
        }
    } while (record1.good && record2.good);

    undetermined1.close();
    undetermined2.close();

    std::cout << records_processed << " total records processed" << "\n";
}
