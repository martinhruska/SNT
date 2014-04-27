PROJ=rcsp_solver

all:
	cd src && make
	mv src/$(PROJ) bin/

run:
	bin/$(PROJ) input/j30/j301_1.sm

clean:
	cd src && make clean
	rm bin/*
