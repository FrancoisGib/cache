#include "cache.h"

char* dec_to_binary(int number) {
    int value = number;
    int8_t size = 1;
    while ((value /= 2) > 0)
        size++;
    char* str = (char*)malloc(size + 1);
    str[size] = '\0';
    while (size > 0) {
        size--;
        str[size] = (number % 2) + '0';
        number /= 2;
    }   
    return str;
}

cache_t* init_cache(int words_per_line, int nb_words, int8_t word_size) {
    cache_t* cache = (cache_t*)malloc(sizeof(cache_t));
    cache->nb_lines = nb_words / words_per_line;
    cache->nb_words = nb_words;
    cache->word_size = word_size;
    cache->lines = (cache_line_t*)malloc(sizeof(cache_line_t) * cache->nb_lines);
    return cache;
}

void free_cache(cache_t* cache) {
    free(cache->lines);
    free(cache);
}

int8_t get_tag_size(cache_t* cache, int8_t mapping_strategy) {
    if (mapping_strategy == DIRECT_MAPPING) // direct mapping
        return unpow(2, cache->nb_words) - unpow(2, cache->nb_lines);
    else if (mapping_strategy == FULLY_ASSOCIATIVE_MAPPING) // fully associative
        //return unpow(cache->nb_words / cache->nb_lines);
        return -1;
    else if (mapping_strategy == SET_ASSOCIATIVE_MAPPING)
        //return unpow(cache->nb_lines) - unpow(cache->nb_lines / cache->nb_words);
        return -1;
    return -1;
}

void gen_input_file() {
    FILE* output = fopen("input", "w");
    srand(time(NULL));
    int8_t nb_max_size = unpow(10, RAM_SIZE);
    char str[nb_max_size + 1];
    for (int i = 0; i < NB_ITERATIONS; i++) {
        int value = rand() % RAM_SIZE;
        value = value < 0 ? -value : value;
        int8_t size = unpow(10, value);
        sprintf(str, "%d\n", value);
        fwrite(str, size + 1, 1, output);
        memset(str, 0, size + 1);
    }
    fclose(output);
}

addresses_t* read_input() {
    FILE* input = fopen("input", "r");
    int size;
    int8_t nb_max_size = unpow(10, RAM_SIZE);
    int value;
    addresses_t* addresses = (addresses_t*)malloc(sizeof(addresses_t));
    addresses->nb_addresses = NB_ITERATIONS;
    addresses->addresses = (int*)malloc(sizeof(int) * addresses->nb_addresses);
    int i = 0;
    while ((size = fscanf(input, "%d", &value)) != -1) {
        printf("%d\n", value);
        addresses->addresses[i] = value;
        i++;
    }
    fclose(input);
    return addresses;
}

void free_addresses(addresses_t* addresses) {
    free(addresses->addresses);
    free(addresses);
}

void add_address_to_cache_direct_mapping(cache_t* cache, int address) {
    int line_number = address % cache->nb_lines;
    cache->lines[line_number].valid = 1;
    int8_t tag_size = get_tag_size(cache, DIRECT_MAPPING);
    int8_t address_size = cache->word_size * 8;
    printf("tagsize : %d\nsize: %d\n", tag_size, address_size);
    cache->lines[line_number].tag = address >> (address_size - tag_size);
    cache->lines[line_number].address = address;
    char* bin = dec_to_binary(address);
    printf("address: %s\ntag: %d\nline_number: %d\n\n", bin, cache->lines[line_number].tag, line_number);
    free(bin);
}

int address_in_cache_direct_mapping(cache_t* cache, int address) {
    int line_number = address % cache->nb_lines;
    int8_t tag_size = get_tag_size(cache, DIRECT_MAPPING);
    int8_t address_size = unpow(2, address);
    int tag = address >> (address_size - tag_size);
    printf("%d, %d\n", cache->lines[line_number].tag, tag);
    return cache->lines[line_number].tag == tag;
}

void print_cache_direct_mapping(cache_t* cache) {
    printf("lines: ");
    for (int i = 0; i < cache->nb_words / cache->nb_lines; i++)
        printf("%d i", i);
    for (int i = 0; i < cache->nb_lines; i++) {
        printf("\n\n");
        int8_t tag_size = get_tag_size(cache, DIRECT_MAPPING);
        int address = cache->lines[i].address;
        for (int j = 0; j < cache->nb_words / cache->nb_lines; j++)
            printf("%d ", address + j);
    }
}

int main() {
    cache_t* cache = init_cache(16, 512, 1);
    gen_input_file();
    addresses_t* addresses = read_input();
    int success = 0;
    int fails = 0;
    for (int i = 0; i < addresses->nb_addresses; i++)
        add_address_to_cache_direct_mapping(cache, addresses->addresses[i]);
    for (int i = 0; i < addresses->nb_addresses; i++)
        if (address_in_cache_direct_mapping(cache, addresses->addresses[i]))
            success += 1;
        else
            fails += 1;
    printf("success: %d, fails: %d\n", success, fails);
    print_cache_direct_mapping(cache);
    free_addresses(addresses);
    free_cache(cache);
    return 0;
}