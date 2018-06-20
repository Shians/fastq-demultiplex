# Paired end FASTQ demultiplexing

Demultiplex paired end reads by fixed position barcodes. This library requires C++17 and Boost C++ libraries. This software is work in progress.

## Installation

This software has successfully compiled on:

* GCC 6.3.0
* clang 6.0.0

Because `gcc` and `clang` use different libraries to handle filesystem. The compilation commands will be different.

To compile the program (for GCC):

```
git clone https://github.com/Shians/fastq-demultiplex.git
cd fastq-demultiplex
make   # if using gcc
```

To compile the program (for clang):

```
git clone https://github.com/Shians/fastq-demultiplex.git
cd fastq-demultiplex
make -f makefile.clang  # if using clang
```

The binaries will be in the `bin` folder. Manually place this in the path or run

```
make install
```

to install to /usr/local/bin, otherwise run

```
make install prefix=/some/path  # installs to /some/path/bin
```

## Usage

Basic usage:

```
paired_fastq_demultiplex \
    --bc "sample_index.csv" \    # comma separated filename, barcode pairs. i.e. "sample1,AGATGATG"
    --r1 "sample_R1.fastq.gz" \  # R1 fastq file
    --r2 "sample_R2.fastq.gz" \  # R2 fastq file
    --first 7 \                  # Barcode starting position (1-indexed). Position on R1 unless --r2bc argument is set
    --last 14 \                  # Barcode ending position
    -o split_files               # Folder to store output
```

See `paired_fastq_demultiplex --help` for details.

Files will be created for each entry in `sample_index.csv`, all reads with no barcode match will be placed in `Undetermined_R1.fastq.gz` and `Undetermined_R2.fastq.gz`.

## Warnings

* On MacOS the number of files that can be opened by `terminal` is limited to 256, this program will fail silently and output fewer than expected files.
    * Easiest way to fix this is by installing iTerm2 and running this program inside.
    * Check your files open limit using `ulimit -n`.
* This is still a work in progress, please report any bugs you find using the issues tab.
