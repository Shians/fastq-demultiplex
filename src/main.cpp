#include "main.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    check_args();
    print_args();

    Timer timer;
    timer.start();

    vector<string> barcodes = read_barcodes(PRG_OPTS.bc);
    sort(barcodes.begin(), barcodes.end());

    demultiplex(barcodes);

    cout << "Time elapsed: " << timer.time_elapsed() << "\n";

    return 0;
}
