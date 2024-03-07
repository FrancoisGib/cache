#include "lib.h"

int8_t unpow(int power, int value) {
    int8_t cpt = (value % power == 0) && (value % 10 != 0) ? 0 : 1;
    while ((value /= power) > 0)
        cpt++;
    return cpt;
}
