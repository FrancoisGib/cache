cache:
	gcc -c cache.c
	gcc -o cache cache.o

clean:
	rm cache cache.o