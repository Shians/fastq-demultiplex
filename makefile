prefix ?= /usr/local

PROG_NAME = paired_fastq_demultiplex

TEST_ARGS = --bc "test/data/sample_index.csv" \
			--r1 "test/data/sample_R1.fastq.gz" \
			--r2 "test/data/sample_R2.fastq.gz" \
			--first 7 \
			--last 14 \
			--r2-bc \
			-o test/output

CPPFLAGS = -o3 -Wall -std=c++11 -I./include -Wno-unused-function -Wno-sign-compare
LIBS = -lz -lboost_system -lboost_filesystem
LDFLAG = 

vpath %.c src
vpath %.o src
vpath %.hpp include
vpath %.h include

files := $(notdir $(wildcard src/*.cpp))
objects := $(addprefix src/,$(files:%.cpp=%.o))

.PHONY: all debug build test install clean

all: build

debug: CPPFLAGS = -Wall -std=c++11 -I./include -Wno-unused-function -Wno-sign-compare -pg -D_DEBUG
debug: build

build: bin/$(PROG_NAME)

test:
	mkdir -p test/output
	bin/$(PROG_NAME) $(TEST_ARGS)

bin/$(PROG_NAME) : $(objects)
	mkdir -p bin
	$(CXX) $(CPPFLAGS) -o bin/$(PROG_NAME) $^ $(LIBS)

src/%.o : %.c
	$(CXX) $(CPPFLAGS) -c -o $@ $< $(LIBS)

install:
	install bin/$(PROG_NAME) $(prefix)/bin/$(PROG_NAME)

clean:
	rm -f src/*.o
	rm -rf test/output
