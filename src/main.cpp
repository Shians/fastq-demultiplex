#include  "main.h"

using namespace std;

vector<string> read_barcodes(string bc_file_name) {
    ifstream barcode_file(bc_file_name);

    vector<string> barcodes;

    string line;
    while (barcode_file >> line) {
        stringstream ss(line);

        string name, barcode;
        getline(ss, name, ',');
        getline(ss, barcode);

        barcodes.push_back(barcode);
    }

    return barcodes;
}

void demultiplex_fastq(
    vector<string> barcodes,
    options_t PRG_OPTS
) {
    string r1 = PRG_OPTS.r1;
    string r2 = PRG_OPTS.r2;
    int bc_start = PRG_OPTS.bc_start;
    int bc_end = PRG_OPTS.bc_end;
    bool r2_bc = PRG_OPTS.r2_bc;
    string outdir = PRG_OPTS.outdir;
    
    GzipFiles files(barcodes);

    Fastq_file fq1(r1);
    Fastq_file fq2(r2);

    for (int i = 0; i < 1000; i++) {
        Fastq_record record1 = fq1.get_record();
        Fastq_record record2 = fq2.get_record();

        string barcode;
        if (!r2_bc) {
            barcode = record1.seq_substr(bc_start, bc_end);
        } else {
            barcode = record2.seq_substr(bc_start, bc_end);
        }

        if (binary_search(barcodes.begin(), barcodes.end(), barcode)) {
            files.get_file1(barcode).write(record1.str());
            files.get_file2(barcode).write(record2.str());
        }
    }
}

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    check_args();

    cout << "Barcodes:" << PRG_OPTS.bc << "\n";
    cout << "R1 file: " << PRG_OPTS.r1 << "\n";
    cout << "R2 file: " << PRG_OPTS.r2 << "\n";
    cout << "Barcode range: " 
         << PRG_OPTS.bc_start << " to " << PRG_OPTS.bc_end 
         << " (" << PRG_OPTS.bc_end - PRG_OPTS.bc_start + 1 << " bases)"
         << " in " << (PRG_OPTS.r2_bc ? "R2" : "R1") << "\n";
    cout << "Output dir: " << PRG_OPTS.outdir << "\n";

    vector<string> barcodes = read_barcodes(PRG_OPTS.bc);
    sort(barcodes.begin(), barcodes.end());

    demultiplex_fastq(barcodes, PRG_OPTS);

    return 0;
}