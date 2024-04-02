#include "cache.h"

cache_t* init_cache(int words_per_line, int nb_words, int8_t word_size, int8_t associativity) {
    cache_t* cache = (cache_t*)malloc(sizeof(cache_t));
    cache->nb_words = nb_words;
    cache->word_size = word_size;
    if (associativity == 0) {
        cache->nb_lines = nb_words / words_per_line;
        cache->lines = (cache_line_t*)malloc((sizeof(cache_line_t)) * cache->nb_lines);
        for (int i = 0; i < cache->nb_lines; i++) {
            cache->lines[i].blocks = (int*)malloc(words_per_line * sizeof(int));
            cache->lines[i].valid = 0;
            cache->lines[i].recentness = 0;
        }
    }
    else {
        cache->nb_lines = nb_words / words_per_line;
        cache->nb_sets = nb_words / (words_per_line * associativity);
        cache->associativity = associativity;
        int nb_lines_per_set =  cache->nb_lines / cache->nb_sets;
        int nb_blocks_per_line = cache->nb_words / (cache->nb_sets * nb_lines_per_set);
        printf("\nblock %d\n", nb_blocks_per_line);
        cache->sets = (cache_set_t*)malloc((sizeof(cache_set_t)) * cache->nb_sets);
        for (int i = 0; i < cache->nb_sets; i++) {
            cache->sets[i].lines = (cache_line_t*)malloc((sizeof(cache_line_t)) * nb_lines_per_set);
            for (int j = 0; j < nb_lines_per_set; j++) {
                cache->sets[i].lines[j].valid = 0;
                cache->sets[i].lines[j].recentness = 0;
                cache->sets[i].lines[j].blocks = (int*)malloc(nb_blocks_per_line * sizeof(int));
            }
        }
    }
    
    return cache;
}

void free_cache(cache_t* cache) {
    if (cache->associativity == 0) {
        for (int i = 0; i < cache->nb_lines; i++)
                free(cache->lines[i].blocks);
            free(cache->lines);
    }
    else {
        for (int i = 0; i < cache->nb_sets; i++) {
            for (int j = 0; j < cache->associativity; j++)
                free(cache->sets[i].lines[j].blocks);
            free(cache->sets[i].lines);
        }
        free(cache->sets);
    }
    free(cache);
}

int8_t insert_if_not_full(cache_t* cache, int address) {
    for (int i = 0; i < cache->nb_lines; i++) {
        if (!cache->lines[i].valid) {
            for (int j = 0; j < cache->nb_words / cache->nb_lines; j++)
                cache->lines[i].blocks[j] = address + j;
            cache->lines[i].recentness = 0;
            cache->lines[i].valid = 1;
            return 1;
        }
    }
    return 0;
}

void print_cache(cache_t* cache, int8_t mapping_type) {
    for (int i = 0; i < cache->nb_words / cache->nb_lines; i++)
        printf("i%d ", i);
    for (int i = 0; i < cache->nb_lines; i++) {
        printf("\n");
        if (cache->lines[i].valid) {
            int* blocks = cache->lines[i].blocks;
            for (int j = 0; j < cache->nb_words / cache->nb_lines; j++)
                printf("%d ", blocks[j]);
            if (mapping_type == FULL_ASSOCIATIVE)
                printf(" - Recentness: %d", cache->lines[i].recentness);
        }
        else
            printf("--------------------"); // unused line
    }
    printf("\n");
}

int8_t is_full(cache_t* cache) {
    for (int i = 0; i < cache->nb_lines; i++) {
        if (!cache->lines[i].valid) 
            return 0;
    }
    return 1;
}

void insert_addresses_in_line(cache_t* cache, int line_number, int address) {
    for (int i = 0; i < cache->nb_words / cache->nb_lines; i++)
        cache->lines[line_number].blocks[i] = address + i;
    cache->lines[line_number].valid = 1;
    cache->lines[line_number].recentness = 0;
}

int get_first_free_line_index(cache_t* cache) {
    for (int i = 0; i < cache->nb_lines; i++) {
        if (!cache->lines[i].valid)
            return i;
    }
    return -1;
}

