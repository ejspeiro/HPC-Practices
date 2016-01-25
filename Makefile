all: store pipelining

store: store.c
	gcc -O2 -S store.c

pipelining: pipelining.o pipelining.c
	gcc pipelining.o -o pipelining

pipelining.o: pipelining.c
	gcc -c pipelining.c

run_pipelining:
	./pipelining >& pipelining_results.txt

clean:
	rm -f *~ *.s *.o pipelining
