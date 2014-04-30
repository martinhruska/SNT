CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -isystem sat_solver
#CFLAGS=-Wall -Wextra -std=c++11 -Isat_solver
MAIN=rcps_solver
PARSER=rcps_parser
GRAPH=rcps_graph
FWALG=rcps_floyd_warshall
SAT=rcps_sat_model
SAT_SOLVER=sat_solver/core/Solver
MODEL=rcps_model_to_glucose
OPTIMIZER=rcps_optimizer

GRAPH_COMP=$(FWALG).cc
FILES=$(MAIN).cc $(PARSER).o $(GRAPH).o $(GRAPH_COMP) $(SAT).o $(SAT_SOLVER).o $(MODEL).o $(OPTIMIZER).o

all: $(MAIN)

$(MAIN): $(FILES) $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(FILES) -o $@

$(PARSER).o: $(PARSER).cc
	$(CC) $(CFLAGS) $(PARSER).cc -c

$(GRAPH).o: $(GRAPH).cc $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(GRAPH).cc $(GRAPH_COMP) -c

$(SAT).o: $(SAT).cc
	$(CC) $(CFLAGS) $(SAT).cc -c

$(MODEL).o: $(MODEL).cc
	$(CC) $(CFLAGS) $(MODEL).cc -c

$(OPTIMIZER).o: $(OPTIMIZER).cc
	$(CC) $(CFLAGS) $(OPTIMIZER).cc -c

$(SAT_SOLVER).o:
	cd sat_solver/core && make

clean:
	cd sat_solver/core && make clean
	rm *.o $(MAIN)
