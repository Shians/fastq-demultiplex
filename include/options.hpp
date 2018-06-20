#pragma once
#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include <algorithm>
#include <boost/filesystem.hpp>
#include <string>
#include <tclap/CmdLine.h>

struct options_t {
    std::string bc;
    std::string r1;
    std::string r2;
    int bc_start;
    int bc_end;
    bool r2_bc;
    std::string outdir;
    int mismatch;
    int complvl;
};

extern options_t PRG_OPTS;

typedef TCLAP::ValueArg<std::string> string_arg;
typedef TCLAP::ValueArg<int> int_arg;
typedef TCLAP::SwitchArg switch_arg;

void parse_args(int &argc, char *argv[]);
void check_args();
void print_args();
