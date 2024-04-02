#include "full_associative.h"

void lru(cache_t* cache, int address) {
   fifo(cache, address); // put is the same as fifo but with lru the get changes the replacement
}

void fifo(cache_t* cache, int address) {
   int line_number = 0;
   cache_line_t* replaced_line = &cache->lines[0];
   int i;
   for (i = 0; i < cache->nb_lines; i++) {
      if (cache->lines[i].recentness > cache->lines[line_number].recentness) 
         line_number = i;
      cache->lines[i].recentness++;
   }
   insert_addresses_in_line(cache, line_number, address);
}

void random_strategy(cache_t* cache, int address) {
   int line_number = rand() % cache->nb_lines;
   insert_addresses_in_line(cache, line_number, address);
}

int get_tag_fully_associative(cache_t* cache, int address) {
   int8_t offset = unpow(2, cache->nb_words / cache->nb_lines); // bits number to distinguish adresses in a block
   int8_t tag_size = cache->word_size - offset;
   return address >> (cache->word_size - tag_size);
}


void add_address_to_cache_fully_associative(cache_t* cache, int address, int8_t algorithm) {
   if (!is_full(cache)) {
      int first_free_line_index = get_first_free_line_index(cache);
      insert_addresses_in_line(cache, first_free_line_index, address);
   }
   else {
      switch (algorithm)
      {
      case RANDOM:
         random_strategy(cache, address);
         break;
      case LRU:
         lru(cache, address);
         break;
      case FIFO:
         fifo(cache, address);
         break;
      }
   }
}

int check_address_in_cache_fully_associative(cache_t* cache, int address) {
   for (int i = 0; i < cache->nb_lines; i++) {
      if (cache->lines[i].valid) {
         int compare_tag = get_tag_fully_associative(cache, cache->lines[i].blocks[0]);
         int address_tag = get_tag_fully_associative(cache, address);
         if (compare_tag == address_tag)
            return i;
      }
   }
   return -1;
}

int lru_get_fully(cache_t* cache, int address) {
   int line_number = check_address_in_cache_fully_associative(cache, address);
   if (line_number != -1) {
      cache->lines[line_number].recentness = 0;
      for (int i = 0; i < cache->nb_lines; i++) 
         cache->lines[i].recentness++;
   }
   return line_number;
}


void compute_success_and_fail_fully_associative(cache_t* cache, int* addresses, int8_t get_strategy) {
   int cycles_count = 0;
   int success = 0;
   int fail = 0;
   get_algorithm* algorithm = get_strategy == LRU ? &lru_get_fully : &check_address_in_cache_fully_associative;
   for (int i = 0; i < NB_ITERATIONS; i++) {
      int line_number;
      if ((line_number = algorithm(cache, addresses[i])) != -1) {
         success++;
         cycles_count += CACHE_ACCESS_TIME;
         int address_tag = get_tag_fully_associative(cache, cache->lines[line_number].blocks[0]);
         char* binary_tag = dec_to_binary(address_tag);
         printf("found %d, tag matches: %s, address in cell: %d\n", addresses[i], binary_tag, cache->lines[line_number].blocks[0]);
         free(binary_tag);
      }
      else {
         fail++;
         cycles_count += RAM_ACCESS_TIME;
         printf("%d not found\n", addresses[i]);
      }
   }
   printf("\nSuccess: %d, fails: %d\nThe total access time is %d cycles\n", success, fail, cycles_count);
}