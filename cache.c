#include "cache.h"

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

void free_addresses(addresses_t* addresses) {
    free(addresses->addresses);
    free(addresses);
}