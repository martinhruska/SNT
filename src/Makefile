CC=g++
CFLAGS=-Wall -Wextra -std=c++11
MAIN=rcps_solver
PARSER=rcps_parser
GRAPH=rcps_graph
FWALG=rcps_floyd_warshall
SAT=rcps_sat_model

GRAPH_COMP=$(FWALG).cc
FILES=$(MAIN).cc $(PARSER).o $(GRAPH).o $(GRAPH_COMP) $(SAT).o

all: $(MAIN)

$(MAIN): $(FILES) $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(FILES) -o $@

$(PARSER).o: $(PARSER).cc
	$(CC) $(CFLAGS) $(PARSER).cc -c

$(GRAPH).o: $(GRAPH).cc $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(GRAPH).cc $(GRAPH_COMP) -c

$(SAT).o: $(SAT).cc
	$(CC) $(CFLAGS) $(SAT).cc -c

clean:
	rm *.o $(MAIN)
