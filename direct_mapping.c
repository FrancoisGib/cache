#include "direct_mapping.h"

int get_tag_direct_mapping(cache_t* cache, int address) {
   int8_t tag_size = unpow(2, cache->nb_lines);
   printf("rag tzi: %d\n", tag_size);
   return address >> (cache->word_size - tag_size);
}

int get_address_line_direct_mapping(cache_t* cache, int address) {
   int line_number = (address / cache->nb_lines) - 1;
   return line_number == -1 ? 0 : line_number;
}

void add_address_to_cache_direct_mapping(cache_t* cache, int address) {
   int line_number = get_address_line_direct_mapping(cache, address);
   cache->lines[line_number].valid = 1;
   cache->lines[line_number].tag = get_tag_direct_mapping(cache, address);
   char* str = dec_to_binary(cache->lines[line_number].tag);
   printf("line %d tag %s\n", line_number, str);
   free(str);
   cache->lines[line_number].address = address;
   cache->lines[line_number].valid = 1;
   char* binary_address = dec_to_binary(address);
   char* binary_tag = dec_to_binary(cache->lines[line_number].tag);
   printf("address: %s\ntag: %d = %s\nline_number: %d\n\n", binary_address, cache->lines[line_number].tag, binary_tag, line_number);
   free(binary_address);
   free(binary_tag);
}

void compute_success_and_fail_direct_mapping(cache_t* cache, addresses_t* addresses) {
   int cycles_count = 0;
   int success = 0;
   int fail = 0;
   for (int i = 0; i < addresses->nb_addresses; i++) {
      int line_number = get_address_line_direct_mapping(cache, addresses->addresses[i]);
      int tag = get_tag_direct_mapping(cache, addresses->addresses[i]);
      if (cache->lines[line_number].tag == tag) {
         success++;
         cycles_count += CACHE_ACCESS_TIME;
         char* binary_tag = dec_to_binary(tag);
         printf("found %d, tag matches: %s, address in cell: %d\n", addresses->addresses[i], binary_tag, cache->lines[line_number].address);
         free(binary_tag);
      }
      else {
         fail++;
         cycles_count += RAM_ACCESS_TIME;
         printf("not found %d, address in cache: %d\n", addresses->addresses[i], cache->lines[line_number].address);
      }
   }
   printf("\nSuccess: %d, fails: %d\nThe total access time is %d cycles\n", success, fail, cycles_count);
}

void print_cache_direct_mapping(cache_t* cache) {
   for (int i = 0; i < cache->nb_words / cache->nb_lines; i++)
      printf("i%d ", i);
   for (int i = 0; i < cache->nb_lines; i++) {
      printf("\n");
      if (cache->lines[i].valid) {
         int address = cache->lines[i].address;
         for (int j = 0; j < cache->nb_words / cache->nb_lines; j++)
               printf("%d ", address + j);
      }
      else
         printf("--------------------"); // unused line
   }
   printf("\n");
}

#define NUMBER_SIZE 2
#define NB_ITERATIONS 4

int main() {
   cache_t* cache = init_cache(2, 2, NUMBER_SIZE);
   printf("lines %d\n", cache->nb_lines);
   //gen_input_file(NB_ITERATIONS, NUMBER_SIZE);
   addresses_t* addresses = read_input(NB_ITERATIONS);
   int success = 0;
   int fails = 0;
   for (int i = 0; i < addresses->nb_addresses; i++)
      add_address_to_cache_direct_mapping(cache, addresses->addresses[i]);
   print_cache_direct_mapping(cache);
   compute_success_and_fail_direct_mapping(cache, addresses);
   free_addresses(addresses);
   free_cache(cache);
   return 0;
}