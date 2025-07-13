// latency_test.c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define SIZE 1000000

uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ (
        "rdtsc"
        : "=a" (lo), "=d" (hi)
    );
    return ((uint64_t)hi << 32) | lo;
}

int main() {
    int *arr = (int *)malloc(SIZE * sizeof(int));

    // Touch all elements to load into memory
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }

    // Start timer
    uint64_t start = rdtsc();
    
    // Access elements
    volatile int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += arr[i];
    }

    // End timer
    uint64_t end = rdtsc();

    printf("Time taken to access %d integers: %lu CPU cycles\n", SIZE, end - start);
    free(arr);
    return 0;
}
