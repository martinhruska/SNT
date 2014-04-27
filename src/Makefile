CC=g++
CFLAGS=-Wall -Wextra -std=c++11
MAIN=rcsp_solver
PARSER=rcsp_parser

FILES=$(MAIN).cc $(PARSER).o

all: $(MAIN)

$(MAIN): $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o $@

$(PARSER).o: $(PARSER).cc
	$(CC) $(CFLAGS) $(PARSER).cc -c

clean:
	rm *.o $(MAIN)
