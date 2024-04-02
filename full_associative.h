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

typedef void replacement_algorithm(cache_t* cache, int address); 

void lru(cache_t* cache, int address);
void fifo(cache_t* cache, int address);
void random_strategy(cache_t* cache, int address);

int lru_get_fully(cache_t* cache, int address);
#define LRU_GET_FULLY &lru_get_fully
int check_address_in_cache_fully_associative(cache_t* cache, int address);
#define NORMAL_GET &check_address_in_cache_fully_associative

int get_tag_fully_associative(cache_t* cache, int address);
void add_address_to_cache_fully_associative(cache_t* cache, int address, int8_t algorithm);
void compute_success_and_fail_fully_associative(cache_t* cache, int* addresses, int8_t get_strategy);