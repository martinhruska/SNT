PROJ=rcps_solver

all:
	cd src && make
	cp src/$(PROJ) bin/

doc:
	cd docs && make

run:
	bin/$(PROJ) input/j30/j301_1.sm

clean:
	cd src && make clean
	cd docs && make clean
	rm bin/*
