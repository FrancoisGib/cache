#include "direct_mapping.h"

int get_tag_direct_mapping(cache_t* cache, int address) {
   int8_t offset = unpow(2, cache->nb_words / cache->nb_lines) - 1; // bits number to distinguish adresses in a block
   int8_t index = unpow(2, cache->nb_lines); // bits number to distinguish lines in the cache
   int8_t tag_size = cache->word_size - (index + offset);
   return address >> (cache->word_size - tag_size);
}

void add_address_to_cache_direct_mapping(cache_t* cache, int address) {
   int line_number = address % cache->nb_lines;
   cache->lines[line_number].valid = 1;
   insert_addresses_in_line(cache, line_number, address);
}

void compute_success_and_fail_direct_mapping(cache_t* cache, int* addresses) {
   int cycles_count = 0;
   int success = 0;
   int fail = 0;
   for (int i = 0; i < NB_ITERATIONS; i++) {
      int line_number = addresses[i] % cache->nb_lines;
      int tag = get_tag_direct_mapping(cache, addresses[i]);
      if (get_tag_direct_mapping(cache, cache->lines[line_number].blocks[0]) == tag) {
         success++;
         cycles_count += CACHE_ACCESS_TIME;
         char* binary_tag = dec_to_binary(tag);
         printf("found %d, tag matches: %s, address in cell: %d\n", addresses[i], binary_tag, cache->lines[line_number].blocks[0]);
         free(binary_tag);
      }
      else {
         fail++;
         cycles_count += RAM_ACCESS_TIME;
         printf("%d not found, address in cache: %d\n", addresses[i], cache->lines[line_number].blocks[0]);
      }
   }
   printf("\nSuccess: %d, fails: %d\nThe total access time is %d cycles\n", success, fail, cycles_count);
}