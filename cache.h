#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define RAM_ACCESS_TIME 100
#define CACHE_ACCESS_TIME 1

#define DIRECT_MAPPING 0
#define FULLY_ASSOCIATIVE_MAPPING 1
#define SET_ASSOCIATIVE_MAPPING 2

typedef struct {
    int8_t valid;
    int tag;
    int index;
} cache_line_t;

typedef struct {
    int nb_lines;
    int nb_words;
    int8_t word_size;
    cache_line_t *lines;
} cache_t;

cache_t* init_cache(int nb_lines, int nb_words, int8_t word_size);
void free_cache(cache_t* cache);