PROJ=rcps_solver

all:
	cd src && make
	cp src/$(PROJ) bin/
#cd src && rm *.o

run:
	bin/$(PROJ) input/j30/j301_1.sm

configure_merlin:
	export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/usr/local/lib64:/usr/lib64

clean:
	cd src && make clean
	rm bin/*
