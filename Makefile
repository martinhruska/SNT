PROJ=rcps_solver

all:
	cd src && make
	cp src/$(PROJ) bin/

run:
	bin/$(PROJ) input_example/j301_1.sm

clean:
	rm bin/*
	cd src && make clean
