#include "set_associative.h"

int get_set_index(cache_t* cache, int address) {
   int8_t offset = unpow(2, cache->nb_words / cache->nb_lines); // bits number to distinguish adresses in a block
   int mask = cache->nb_sets - 1;
   int index = unpow(2, cache->nb_sets);
   int tag_size = cache->word_size - (index + offset);
   return address >> offset & mask;
}

int get_tag_set_associative(cache_t* cache, int address) {
   int8_t offset = unpow(2, cache->nb_words / cache->nb_lines); // bits number to distinguish adresses in a block
   int8_t index = unpow(2, cache->nb_sets);
   int8_t tag_size = cache->word_size - (index + offset);
   return address >> (cache->word_size - tag_size);
   return offset;
}

void add_address_to_cache_set_associative(cache_t* cache, int address, int8_t algorithm) {
   int set_number = get_set_index(cache, address);

   switch (algorithm) {
      case RANDOM:
         random_set(cache, set_number, address);
         break;
      case LRU:
         lru_set(cache,set_number, address);
         break;
      case FIFO:
         fifo_set(cache, set_number, address);
         break;
   }
}

void lru_set(cache_t* cache, int set_number, int address) {
   fifo_set(cache, set_number, address); // put is the same as fifo but with lru the get changes the replacement
}

int lru_get_set(cache_t* cache, int address) {
   int set_number = get_set_index(cache, address);
   int line_number = 0;
   int nb_lines_per_set = get_lines_per_set(cache);
   for (int i = 0; i < nb_lines_per_set; i++) {
      cache_line_t* line = &cache->sets[set_number].lines[i];
      if (line->valid) {
         int tag = get_tag_set_associative(cache, address);
         if (tag == get_tag_set_associative(cache, line->blocks[0])) {
            line_number = i;
            line->recentness = 0;
         }
      }
      else 
         line->recentness++;
   }
   return line_number;
}

void fifo_set(cache_t* cache, int set_number, int address) {
   int line_number = 0;
   int nb_lines_per_set = get_lines_per_set(cache);
   int i;
   for (i = 0; i < nb_lines_per_set; i++) {
      if (cache->sets[set_number].lines[i].recentness > cache->sets[set_number].lines[line_number].recentness) 
         line_number = i;
      cache->sets[set_number].lines[i].recentness++;
   }
   insert_addresses_in_set(cache, set_number, line_number, address);
}

void random_set(cache_t* cache, int set_number, int address) {
   int i;
   if (set_is_full(cache, set_number)) {
      int nb_lines_per_set = get_lines_per_set(cache);
      int i = rand() % nb_lines_per_set;
   }
   else {
      int i = 0;
      while (cache->sets[set_number].lines[i].valid)
         i++;
   }
   insert_addresses_in_set(cache, set_number, i, address);
}


void insert_addresses_in_set(cache_t* cache, int set_number, int line_number, int address) {
   int nb_blocks_per_line = get_blocks_per_line(cache);
   for (int i = 0; i < nb_blocks_per_line; i++)
      cache->sets[set_number].lines[line_number].blocks[i] = address + i;
   cache->sets[set_number].lines[line_number].valid = 1;
   cache->sets[set_number].lines[line_number].recentness = 0;
}

void print_cache_set_associative(cache_t* cache) {
   int nb_lines_per_set =  get_lines_per_set(cache);
   int nb_blocks_per_line = get_blocks_per_line(cache);
   printf("\n");
   for (int i = 0; i < cache->nb_words / cache->nb_lines; i++)
      printf("i%d ", i);
   for (int i = 0; i < cache->nb_sets; i++) {
      printf("\nSet %d\n", i);
      for (int j = 0; j < nb_lines_per_set; j++) {
         if (cache->sets[i].lines[j].valid) {
            int* blocks = cache->sets[i].lines[j].blocks;
            int nb_blocks_per_line = cache->nb_words / (cache->nb_sets * nb_lines_per_set);
            for (int k = 0; k < nb_blocks_per_line; k++)
               printf("%d ", cache->sets[i].lines[j].blocks[k]);
            printf("\n");
         }
         else
            printf("--------------------"); // unused line
      }
   }
   printf("\n");
}

int8_t set_is_full(cache_t* cache, int set_number) {
   int nb_lines_per_set =  cache->nb_lines / cache->nb_sets;
   for (int i = 0; i < nb_lines_per_set; i++) {
      if (!cache->sets[set_number].lines[i].valid)
         return 0;
   }
   return 1;
}

int get_lines_per_set(cache_t* cache) {
   return cache->nb_lines / cache->nb_sets;
}

int get_blocks_per_line(cache_t* cache) {
   return cache->nb_words / (cache->nb_sets * get_lines_per_set(cache));
}

int check_address_in_cache_set_associative(cache_t* cache, int address) {
   int set_number = get_set_index(cache, address);
   for (int i = 0; i < get_lines_per_set(cache); i++) {
      if (cache->sets[set_number].lines[i].valid) {
         int compare_tag = get_tag_set_associative(cache, cache->sets[set_number].lines[i].blocks[0]);
         int address_tag = get_tag_set_associative(cache, address);
         if (compare_tag == address_tag)
            return i;
      }
   }
   return -1;
}

void compute_success_and_fail_set_associative(cache_t* cache, int* addresses, int8_t get_strategy) {
   int cycles_count = 0;
   int success = 0;
   int fail = 0;
   get_algorithm* algorithm = get_strategy == LRU ? &lru_get_set : &check_address_in_cache_set_associative;
   for (int i = 0; i < NB_ITERATIONS; i++) {
      int line_number;
      if ((line_number = algorithm(cache, addresses[i])) != -1) {
         success++;
         cycles_count += CACHE_ACCESS_TIME;
         int set_number = get_set_index(cache, addresses[i]);
         int address_tag = get_tag_set_associative(cache, cache->sets[set_number].lines[line_number].blocks[0]);
         char* binary_tag = dec_to_binary(address_tag);
         printf("found %d, tag matches: %s, address in cell: %d\n", addresses[i], binary_tag, cache->sets[set_number].lines[line_number].blocks[0]);
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