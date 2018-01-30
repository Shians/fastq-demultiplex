#pragma once
#include <string>
#include <tclap/CmdLine.h>
#include <boost/filesystem.hpp>

struct options_t {
    std::string bc;
    std::string r1;
    std::string r2;
    int bc_start;
    int bc_end;
    bool r2_bc;
    std::string outdir;
} PRG_OPTS;

typedef TCLAP::ValueArg<std::string> string_arg;
typedef TCLAP::ValueArg<int> int_arg;
typedef TCLAP::SwitchArg switch_arg;

void parse_args(int &argc, char *argv[]) {
    try {  
        TCLAP::CmdLine cmd("Command description message", ' ', "0.9");

        // arguments are pushed into order, so inserted from last to first
        string_arg outdir("o", "outdir", "output folder", false, "output", "", cmd);
        switch_arg r2_bc("", "r2-bc", "set flag if barcode is on R2", cmd, false);
        int_arg bc_end("l", "last", "barcode end location", true, 0, "", cmd);
        int_arg bc_start("f", "first", "barcode start location", true, 0, "", cmd);
        string_arg r2("", "r2", "R2 file", true, "", "gzipped fastq", cmd);
        string_arg r1("", "r1", "R1 file", true, "", "gzipped fastq", cmd);
        string_arg bc("", "bc", "barcode file", true, "", "csv", cmd);

        cmd.parse(argc, argv);

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
    bool folders_missing = false;
    
    if (!boost::filesystem::exists(PRG_OPTS.r1)) {
        std::cerr << "R1 does not exist" << std::endl;
        files_missing = true;
    }

    if (!boost::filesystem::exists(PRG_OPTS.r2)) {
        std::cerr << "R2 doest no exist" << std::endl;
        files_missing = true;
    }

    if (!boost::filesystem::is_directory(PRG_OPTS.outdir)) {
        std::cerr << "output directory does not exist" << std::endl;
        folders_missing = true;
    }

    if (files_missing || folders_missing) {
        exit(EXIT_FAILURE);
    }
}

void print_args() {
    std::cout << "Barcodes:" << PRG_OPTS.bc << "\n";
    std::cout << "R1 file: " << PRG_OPTS.r1 << "\n";
    std::cout << "R2 file: " << PRG_OPTS.r2 << "\n";
    std::cout << "Barcode range: " 
        << PRG_OPTS.bc_start << " to " << PRG_OPTS.bc_end 
        << " (" << PRG_OPTS.bc_end - PRG_OPTS.bc_start + 1 << " bases)"
        << " in " << (PRG_OPTS.r2_bc ? "R2" : "R1") << "\n";
    std::cout << "Output dir: " << PRG_OPTS.outdir << "\n";
}
