#include<stdio.h>
#include "vec.h"

int main() {
    Vec(int) array;
    vec_init(array);
    vec_push(int, array, 10);
    vec_push(int, array, 20);
    vec_push(int, array, 30);
    vec_pop(array);
    for (size_t i = 0; i < vec_size(array); i++) {
        printf("%d\n", vec_A(array, i));
    }

    vec_destroy(array);
    return 0;
}