#include "cache.h"

// tag = unpow(nb_words) - unpow(nb_lines) - unpow(nb_words_per_line)

int8_t unpow(int16_t value) {
    int8_t cpt = 0;
    while ((value = value / 2) > 0)
        cpt++;
    return cpt;
}

int8_t mpow(int value) {
    int8_t cpt = 0;
    while ((value /= 2) > 0)
        cpt++;
    return cpt;
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

int8_t tag_size(cache_t* cache, int8_t mapping_strategy) {
    if (mapping_strategy == DIRECT_MAPPING) // direct mapping
        return unpow(cache->nb_words) - unpow(cache->nb_lines);
    else if (mapping_strategy == FULLY_ASSOCIATIVE_MAPPING) // fully associative
        return unpow(cache->nb_words / cache->nb_lines);
    else if (mapping_strategy == SET_ASSOCIATIVE_MAPPING)
        return unpow(cache->nb_lines) - unpow(cache->nb_lines / cache->nb_words);
    return -1;
}

int main() {
    int ram_words = mpow(16384);
    cache_t* cache = init_cache(16, 16384, 2);
    printf("%d\n", ram_words);
    free(cache);
    return 0;
}