#include "direct_mapping.h"
#include "full_associative.h"
#include "set_associative.h"

typedef struct {
   int8_t mapping_type;
   int8_t replacement_algorithm;
   int8_t associativity;
} cache_strategy;

cache_t* create_cache(int words_per_line, int nb_words, int8_t word_size, int8_t associativity) {
   return init_cache(words_per_line, nb_words, word_size, associativity);
}

void add_address_to_cache(cache_t* cache, int address, cache_strategy strategy) {
   switch (strategy.mapping_type) {
      case DIRECT_MAPPING:
         add_address_to_cache_direct_mapping(cache, address);
         break;
      case FULL_ASSOCIATIVE:
         add_address_to_cache_fully_associative(cache, address, strategy.replacement_algorithm);
         break;
      case SET_ASSOCIATIVE:
         add_address_to_cache_set_associative(cache, address, strategy.replacement_algorithm);
   }
}

void compute_success_and_fail(cache_t* cache, int* addresses, cache_strategy strategy) {
   switch (strategy.mapping_type) {
      case DIRECT_MAPPING:
         compute_success_and_fail_direct_mapping(cache, addresses);
         break;
      case FULL_ASSOCIATIVE:
         compute_success_and_fail_fully_associative(cache, addresses, strategy.replacement_algorithm);
         break;
      case SET_ASSOCIATIVE:
         compute_success_and_fail_set_associative(cache, addresses, strategy.replacement_algorithm);
   }
}

void print_cache_strategy(cache_t* cache, cache_strategy strategy) {
   if (strategy.mapping_type == SET_ASSOCIATIVE)
      print_cache_set_associative(cache);
   else
      print_cache(cache, strategy.mapping_type);

}

int main(int argc, char* argv[]) {
   cache_strategy strategy = {FULL_ASSOCIATIVE, FIFO, 0};
   cache_t* cache = create_cache(4, 256, NUMBER_SIZE, strategy.associativity);
   gen_input_file(NB_ITERATIONS, NUMBER_SIZE);
   int* addresses = read_input(NB_ITERATIONS);
   for (int i = 0; i < NB_ITERATIONS; i++) 
      add_address_to_cache(cache, addresses[i], strategy);
   print_cache_strategy(cache, strategy);
   compute_success_and_fail(cache, addresses, strategy);
   
   free(addresses);
   free_cache(cache);
   return 0;
}