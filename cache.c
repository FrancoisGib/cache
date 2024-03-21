#include "cache.h"

cache_t* init_cache(int words_per_line, int nb_words, int8_t word_size) {
    cache_t* cache = (cache_t*)malloc(sizeof(cache_t));
    cache->nb_lines = nb_words / words_per_line;
    printf("%d\n", cache->nb_lines);
    printf("test %d\n", unpow(2, cache->nb_words) - unpow(2, cache->nb_lines));
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
        return (cache->word_size * 8) - unpow(2, cache->nb_words) - unpow(2, cache->nb_lines);
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
    printf("tag size %d\n", tag_size);
    cache->lines[line_number].tag = (address >> tag_size);
    cache->lines[line_number].address = address;
    cache->lines[line_number].valid = 1;
    char* binary_address = dec_to_binary(address);
    char* binary_tag = dec_to_binary(cache->lines[line_number].tag);
    printf("%d\n", tag_size);
    printf("address: %s\ntag: %d = %s\nline_number: %d\n\n", binary_address, cache->lines[line_number].tag, binary_tag, line_number);
    free(binary_address);
    free(binary_tag);
}

int access_time = 0;

int address_in_cache_direct_mapping(cache_t* cache, int address) {
    int line_number = address % cache->nb_lines;
    int8_t tag_size = get_tag_size(cache, DIRECT_MAPPING);
    int tag = address >> tag_size;
    int8_t res = cache->lines[line_number].tag == tag;
    access_time += res ? CACHE_ACCESS_TIME : RAM_ACCESS_TIME;
    if (res) {
        char* binary_tag = dec_to_binary(tag);
        printf("found %d, tag matches: %s\n", address, binary_tag);
        free(binary_tag);
    }
    else
        printf("not found %d, address in cache: %d\n", address, cache->lines[line_number].address);
    return res;
}

void print_cache_direct_mapping(cache_t* cache) {
    for (int i = 0; i < cache->nb_words / cache->nb_lines; i++)
        printf("i%d ", i);
    for (int i = 0; i < cache->nb_lines; i++) {
        printf("\n");
        int8_t tag_size = get_tag_size(cache, DIRECT_MAPPING);
        if (cache->lines[i].valid) {
            int address = cache->lines[i].address;
            for (int j = 0; j < cache->nb_words / cache->nb_lines; j++)
                printf("%d ", address + j);
        }
        else
            printf("--------------------"); // empty line
    }
    printf("\nThe total access time is %d cycles\n", access_time);
}

int main() {
    cache_t* cache = init_cache(8, 128, 1);
    gen_input_file();
    addresses_t* addresses = read_input();
    int success = 0;
    int fails = 0;
    for (int i = 0; i < addresses->nb_addresses; i++)
        add_address_to_cache_direct_mapping(cache, addresses->addresses[i]);
    print_cache_direct_mapping(cache);
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