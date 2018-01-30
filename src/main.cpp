#include  "main.h"

using namespace std;

int main(int argc, char* argv[]) {
    parse_args(argc, argv);
    check_args();
    print_args();

    Timer timer;
    timer.start();
    timer.mode("minutes");

    vector<string> barcodes = read_barcodes(PRG_OPTS.bc);
    sort(barcodes.begin(), barcodes.end());

    demultiplex(barcodes, PRG_OPTS);

    std::cout << "Time elapsed: " << timer.elapsed() << std::endl;

    return 0;
}
