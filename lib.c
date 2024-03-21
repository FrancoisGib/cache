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