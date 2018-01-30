PROG_NAME = paired_fastq_demultiplex
PROG_ARGS = --bc "data/sample_index.csv" --r1 "data/sample_R1.fastq.gz" --r2 "data/sample_R2.fastq.gz" --first 7 --last 14 --r2-bc

CPPFLAGS = -o3 -Wall -std=c++14 -I./include -Wno-unused-function -Wno-sign-compare
LIBS = -lz
LDFLAG = 

vpath %.c src
vpath %.o src
vpath %.hpp include
vpath %.h include

files := $(notdir $(wildcard src/*.cpp))
objects := $(addprefix src/,$(files:%.cpp=%.o))

.PHONY: all build test clean

all: build

build: dist/$(PROG_NAME)

test:
	mkdir -p output
	dist/$(PROG_NAME) $(PROG_ARGS)

dist/$(PROG_NAME) : $(objects)
	mkdir -p dist
	$(CXX) $(CPPFLAGS) -o dist/$(PROG_NAME) $^ $(LIBS)

src/%.o : %.c
	$(CXX) $(CPPFLAGS) -c -o $@ $< $(LIBS)

clean:
	rm -f src/*.o
	rm -rf output
