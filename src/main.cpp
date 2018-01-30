#include  "main.h"

using namespace std;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    check_args();
    print_args();

    vector<string> barcodes = read_barcodes(PRG_OPTS.bc);
    sort(barcodes.begin(), barcodes.end());

    demultiplex(barcodes, PRG_OPTS);

    return 0;
}