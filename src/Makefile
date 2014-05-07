CC=g++-4.8
#CFLAGS=-Wall -Wextra -std=c++11 -isystem sat_solver/minisat/ -Lsat_solver/minisat/build/dynamic/lib -lminisat
CFLAGS := -Wall -O3 -Wextra -std=c++11 -isystem sat_solver/glucose/ -isystem sat_solver/glucose/ -isystem sat_solver/minisat/ -Lsat_solver/minisat/build/dynamic/lib -lminisat -D __STDC_LIMIT_MACROS -D __STDC_FORMAT_MACROS
#CFLAGS=-Wall -Wextra -std=c++11 -Isat_solver
MAIN=rcps_solver
PARSER=rcps_parser
GRAPH=rcps_graph
FWALG=rcps_floyd_warshall
SAT=rcps_sat_model
OPTIMIZER=rcps_optimizer

SAT_GLUCOSE=sat_solver/glucose
SAT_SOLVER=$(SAT_GLUCOSE)/core/Solver
MODEL=rcps_model_to_glucose

SAT_MINISAT=sat_solver/minisat/build/release/minisat/
MINISAT_SOLVER=$(SAT_MINISAT)/core/Solver
MODELMINI=rcps_model_to_minisat

GRAPH_COMP=$(FWALG).cc
#FILES=$(MAIN).cc $(PARSER).o $(GRAPH).o $(GRAPH_COMP) $(SAT).o $(MODELMINI).o $(OPTIMIZER).o $(MINISAT_SOLVER).o
FILES=$(MAIN).cc $(PARSER).o $(GRAPH).o $(GRAPH_COMP) $(SAT).o $(SAT_SOLVER).or $(MODEL).o $(OPTIMIZER).o $(MODELMINI).o $(MINISAT_SOLVER).o


all: $(MAIN)

$(MAIN): $(FILES) $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(FILES) -o $@

$(PARSER).o: $(PARSER).cc
	$(CC) $(CFLAGS) $(PARSER).cc -c

$(GRAPH).o: $(GRAPH).cc $(GRAPH_COMP)
	$(CC) $(CFLAGS) $(GRAPH).cc $(GRAPH_COMP) -c

$(SAT).o: $(SAT).cc
	$(CC) $(CFLAGS) $(SAT).cc -c

$(MODELMINI).o: $(MODELMINI).cc
	$(CC) $(CFLAGS) $(MODELMINI).cc -c
$(MODEL).o: $(MODEL).cc
	$(CC) $(CFLAGS) $(MODEL).cc -c

$(OPTIMIZER).o: $(OPTIMIZER).cc
	$(CC) $(CFLAGS) $(OPTIMIZER).cc -c

$(SAT_SOLVER).or: force_look
	cd $(SAT_GLUCOSE)/core && make rs
$(MINISAT_SOLVER).o: force_look
	cd sat_solver/minisat && make

clean:
	cd $(SAT_GLUCOSE)/core && make clean
	cd sat_solver/minisat/ && make clean
	rm *.o $(MAIN)

force_look:
	true
