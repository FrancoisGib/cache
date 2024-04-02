all:
	gcc -c lib.c
	gcc -c cache.c -g
	gcc -c direct_mapping.c -lm
	gcc -c full_associative.c -lm
	gcc -c set_associative.c -lm
	gcc -c main.c -g
	gcc -o main main.o cache.o lib.o direct_mapping.o full_associative.o set_associative.o -lm -g

clean:
	rm -f *.o input main