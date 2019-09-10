#include "main.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    check_args();
    print_args();

    Timer timer;
    timer.start();

    int const expected_length = PRG_OPTS.bc_end - PRG_OPTS.bc_start + 1;
    vector<string> barcodes = read_barcodes(PRG_OPTS.bc, expected_length);
    sort(barcodes.begin(), barcodes.end());

    demultiplex(barcodes);

    cout << "Time elapsed: " << timer.time_elapsed() << "\n";

    return 0;
}
