PROJ=rcps_solver

all:
	cd src && make
	cp src/$(PROJ) bin/
#cd src && rm *.o

minisat:
	cd src && make minisat
	cp src/$(PROJ) bin/
	
run:
	bin/$(PROJ) input/j30/j301_1.sm

clean:
	cd src && make clean
	rm bin/*
