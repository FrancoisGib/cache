#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <math.h>
#include "lib.h"

int get_tag_direct_mapping(cache_t* cache, int address);
void add_address_to_cache_direct_mapping(cache_t* cache, int address);
void compute_success_and_fail_direct_mapping(cache_t* cache, addresses_t* addresses);
void print_cache_direct_mapping(cache_t* cache);