#ifndef INCLUDES
#define INCLUDES
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <math.h>
#include "lib.h"

#endif

int get_set_index(cache_t* cache, int address);
int get_tag_set_associative(cache_t* cache, int address);
void add_address_to_cache_set_associative(cache_t* cache, int address, int8_t algorithm);
void lru_set(cache_t* cache, int set_number, int address);
void fifo_set(cache_t* cache, int set_number, int address);
void random_set(cache_t* cache, int set_number, int address);
void insert_addresses_in_set(cache_t* cache, int set_number, int line_number, int address);
void print_cache_set_associative(cache_t* cache);
int8_t set_is_full(cache_t* cache, int set_number);
int get_blocks_per_line(cache_t* cache);
int get_lines_per_set(cache_t* cache);
void compute_success_and_fail_set_associative(cache_t* cache, int* addresses, int8_t get_strategy);