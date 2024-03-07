all:
	gcc -c lib.c
	make cache

cache:
	gcc -c cache.c
	gcc -o cache cache.o lib.o -lm

clean:
	rm -f *.o cache lib input