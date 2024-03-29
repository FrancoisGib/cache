all:
	gcc -c lib.c
	gcc -c cache.c
	gcc -c direct_mapping.c -lm
	gcc -o direct_mapping cache.o lib.o direct_mapping.o -lm

clean:
	rm -f *.o input direct_mapping