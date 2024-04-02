#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

// for includes
#ifndef CACHE
#define CACHE
#endif

#define RAM_ACCESS_TIME 100
#define CACHE_ACCESS_TIME 1

#define DIRECT_MAPPING 0
#define FULL_ASSOCIATIVE 1
#define SET_ASSOCIATIVE 2

#define NUMBER_SIZE 14
#define NB_ITERATIONS 1000

#define LRU 0
#define FIFO 1
#define RANDOM 2

typedef struct {
    int8_t valid;
    int* blocks;
    int recentness;
} cache_line_t;

typedef struct {
    cache_line_t* lines;
} cache_set_t;

typedef struct {
    int nb_lines;
    int nb_words;
    int8_t word_size;
    cache_line_t* lines;
    cache_set_t* sets;
    int nb_sets;
    int8_t associativity;
} cache_t;

typedef int get_algorithm(cache_t* cache, int address);

cache_t* init_cache(int words_per_line, int nb_words, int8_t word_size, int8_t associativity);
void free_cache(cache_t* cache);
int8_t insert_if_not_full(cache_t* cache, int address);
void print_cache(cache_t* cache, int8_t mapping_type);
int8_t is_full(cache_t* cache);
void insert_addresses_in_line(cache_t* cache, int line_number, int address);
int get_first_free_line_index(cache_t* cache);