#include "options.hpp"

options_t PRG_OPTS;

namespace fs = std::experimental::filesystem;

void parse_args(int &argc, char *argv[]) {
    try {  
        TCLAP::CmdLine cmd("Command description message", ' ', "0.9.2");

        // arguments are pushed into order, so inserted from last to first
        int_arg complvl("z", "complvl", "compression level", false, 9, "int", cmd);
        int_arg mismatch("m", "mismatch", "number of base mismatches allowed for barcodes", false, 1, "int", cmd);
        string_arg outdir("o", "outdir", "output folder", false, "output", "folder", cmd);
        switch_arg r2_bc("", "r2bc", "set flag if barcode is on R2", cmd, false);
        int_arg bc_end("l", "last", "barcode end location", true, 0, "int", cmd);
        int_arg bc_start("f", "first", "barcode start location (1-indexed)", true, 0, "int", cmd);
        string_arg r2("", "r2", "R2 file", true, "", "gzipped fastq", cmd);
        string_arg r1("", "r1", "R1 file", true, "", "gzipped fastq", cmd);
        string_arg bc("", "bc", "barcode file", true, "", "csv", cmd);

        cmd.parse(argc, argv);

        PRG_OPTS.complvl = complvl.getValue();
        PRG_OPTS.mismatch = mismatch.getValue();
        PRG_OPTS.outdir = outdir.getValue();
        PRG_OPTS.r2_bc = r2_bc.getValue();
        PRG_OPTS.bc_end = bc_end.getValue();
        PRG_OPTS.bc_start = bc_start.getValue();
        PRG_OPTS.bc = bc.getValue();
        PRG_OPTS.r2 = r2.getValue();
        PRG_OPTS.r1 = r1.getValue();
	} catch (TCLAP::ArgException &e) {
        throw e;
    }
}

void check_args() {
    bool files_missing = false;
    bool folder_missing = false;
    
    if (!fs::exists(PRG_OPTS.r1)) {
        std::cerr << fmt::format("R1 does not exist: {}\n", PRG_OPTS.r1) << std::endl;
        files_missing = true;
    }

    if (!fs::exists(PRG_OPTS.r2)) {
        std::cerr << fmt::format("R2 does not exist: {}\n", PRG_OPTS.r2)  << std::endl;
        files_missing = true;
    }

    if (!fs::is_directory(PRG_OPTS.outdir)) {
        std::cout << fmt::format("output directory does not exist: {}\n", PRG_OPTS.outdir)  << std::endl;
        folder_missing = true;
    }

    if (files_missing) {
        exit(EXIT_FAILURE);
    }

    if (folder_missing) {
        std::cout << fmt::format("creating directory: {}\n", PRG_OPTS.outdir)  << std::endl;
        fs::create_directory(PRG_OPTS.outdir);
    }
}

void print_args() {
    std::cout << fmt::format("Barcodes file: {}" ,PRG_OPTS.bc) << std::endl;
    std::cout << fmt::format("R1 file: {}", PRG_OPTS.r1) << std::endl;
    std::cout << fmt::format("R2 file: {}", PRG_OPTS.r2) << std::endl;
    std::cout << fmt::format("Barcode range: {} to {} ({} bases in {})",
        PRG_OPTS.bc_start,
        PRG_OPTS.bc_end,
        PRG_OPTS.bc_end - PRG_OPTS.bc_start + 1,
        (PRG_OPTS.r2_bc ? "R2" : "R1")
    ) << std::endl;
    std::cout << fmt::format("Output dir: {}", PRG_OPTS.outdir) << std::endl;
    std::cout << fmt::format("Allowed mismatches: {}", PRG_OPTS.mismatch) << std::endl;
    std::cout << fmt::format("Compression level: {}", PRG_OPTS.complvl) << std::endl;
}
