#include "options.h"

options_t PRG_OPTS;

namespace fs = std::experimental::filesystem;

void parse_args(int &argc, char *argv[]) {
    try {  
        TCLAP::CmdLine cmd("Command description message", ' ', "0.9.1");

        // arguments are pushed into order, so inserted from last to first
        int_arg complvl("", "complvl", "compression level", false, 4, "int", cmd);
        string_arg outdir("o", "outdir", "output folder", false, "output", "folder", cmd);
        switch_arg r2_bc("", "r2bc", "set flag if barcode is on R2", cmd, false);
        int_arg bc_end("l", "last", "barcode end location", true, 0, "int", cmd);
        int_arg bc_start("f", "first", "barcode start location (1-indexed)", true, 0, "int", cmd);
        string_arg r2("", "r2", "R2 file", true, "", "gzipped fastq", cmd);
        string_arg r1("", "r1", "R1 file", true, "", "gzipped fastq", cmd);
        string_arg bc("", "bc", "barcode file", true, "", "csv", cmd);

        cmd.parse(argc, argv);

        PRG_OPTS.complvl = complvl.getValue();
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
        fmt::print(stderr, "R1 does not exist: {}\n", PRG_OPTS.r1);
        files_missing = true;
    }

    if (!fs::exists(PRG_OPTS.r2)) {
        fmt::print(stderr, "R2 does not exist: {}\n", PRG_OPTS.r2);
        files_missing = true;
    }

    if (!fs::is_directory(PRG_OPTS.outdir)) {
        fmt::print(stderr, "output directory does not exist: {}\n", PRG_OPTS.outdir);
        folder_missing = true;
    }

    if (files_missing) {
        exit(EXIT_FAILURE);
    }

    if (folder_missing) {
        fmt::print("creating directory: {}\n", PRG_OPTS.outdir);
        fs::create_directory(PRG_OPTS.outdir);
    }
}

void print_args() {
    fmt::print("Barcodes file: {}\n" ,PRG_OPTS.bc);
    fmt::print("R1 file: {}\n", PRG_OPTS.r1);
    fmt::print("R2 file: {}\n", PRG_OPTS.r2);
    fmt::print("Barcode range: {} to {} ({} bases in {})\n",
        PRG_OPTS.bc_start,
        PRG_OPTS.bc_end,
        PRG_OPTS.bc_end - PRG_OPTS.bc_start + 1,
        (PRG_OPTS.r2_bc ? "R2" : "R1")
    );
    fmt::print("Output dir: {}\n", PRG_OPTS.outdir);
}
