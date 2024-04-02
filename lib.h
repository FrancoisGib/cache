#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#ifndef CACHE
#include "cache.h"
#endif

int8_t unpow(int power, int value);
char* dec_to_binary(int number);
void gen_input_file(int nb_iterations, int number_size);
int* read_input(int nb_iterations);