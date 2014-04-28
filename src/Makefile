CC=g++
CFLAGS=-Wall -Wextra -std=c++11
MAIN=rcsp_solver
PARSER=rcps_parser
GRAPH=rcps_graph
FWALG=rcps_floyd_warshall

GRAPH_COMP=$(FWALG).cc
FILES=$(MAIN).cc $(PARSER).o $(GRAPH).o $(GRAPH_COMP)

all: $(MAIN)

$(MAIN): $(FILES) $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(FILES) -o $@

$(PARSER).o: $(PARSER).cc
	$(CC) $(CFLAGS) $(PARSER).cc -c

$(GRAPH).o: $(GRAPH).cc $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(GRAPH).cc $(GRAPH_COMP) -c

clean:
	rm *.o $(MAIN)
