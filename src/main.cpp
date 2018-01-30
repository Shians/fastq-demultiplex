#include "main.h"

using namespace std;

#if defined(_DEBUG)
    const string TIME_MODE = "milliseconds";
#else
    const string TIME_MODE = "minutes";
#endif

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    check_args();
    print_args();

    Timer timer(TIME_MODE);
    timer.start();

    vector<string> barcodes = read_barcodes(PRG_OPTS.bc);
    sort(barcodes.begin(), barcodes.end());

    demultiplex(barcodes, PRG_OPTS);

    timer.print_elapsed();

    return 0;
}
