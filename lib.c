#include "lib.h"

int8_t unpow(int power, int value) {
    int8_t cpt = (value % power == 0) && (value % 10 != 0) ? 0 : 1;
    while ((value /= power) > 0)
        cpt++;
    return cpt;
}

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

void gen_input_file(int nb_iterations, int number_size) {
    int max_address = (int)pow(2, number_size);
    FILE* output = fopen("input", "w");
    srand(time(NULL));
    int8_t nb_max_size = unpow(10, max_address);
    char str[nb_max_size + 1];
    for (int i = 0; i < nb_iterations; i++) {
        int value = rand() % max_address;
        value = value < 0 ? -value : value;
        int8_t size = unpow(10, value);
        sprintf(str, "%d\n", value);
        fwrite(str, size + 1, 1, output);
        memset(str, 0, size + 1);
    }
    fclose(output);
}

int* read_input(int nb_iterations) {
    FILE* input = fopen("input", "r");
    int size;
    int value;
    int* addresses = (int*)malloc(sizeof(int) * nb_iterations);
    int i = 0;
    while ((size = fscanf(input, "%d", &value)) != -1) {
        printf("%d\n", value);
        addresses[i] = value;
        i++;
    }
    fclose(input);
    return addresses;
}