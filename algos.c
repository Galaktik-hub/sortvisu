#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swap(int* x, int* y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}


void shuffle_arr(int arr[], size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() % (n - i);
            swap(&arr[i], &arr[j]);
        }
    }
}

void print_arr(int arr[], size_t n) {
    if (n == 0)
        return;
    
    for (size_t i = 0; i < n - 1; i++)
        printf("%d, ", arr[i]);

    printf("%d\n", arr[n - 1]);
}

void bubble_sort(int arr[], size_t n) {
    size_t i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = false;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = true;
            }
        }

        if (swapped == false)
            break;
    }
}

void selection_sort(int arr[], size_t n) {
    // TODO : Implémenter le selection sort
}

void insertion_sort(int arr[], size_t n) {
    // TODO : Implémenter le insertion sort
}

void quick_sort(int arr[], int low, int high) {
    // TODO : Implémenter le quick sort
}

void merge_sort(int arr[], size_t n) {
    // TODO : Implémenter le merge sort
}

int main() {
    srand(time(NULL));

    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    printf("Original arr :\n");
    print_arr(arr, n);

    shuffle_arr(arr, n);
    printf("Shuffled arr :\n");
    print_arr(arr, n);

    bubble_sort(arr, n);
    printf("Bubble sort :\n");
    print_arr(arr, n);

    /*shuffle_arr(arr, n);
    selection_sort(arr, n);
    printf("Selection sort :\n");
    print_arr(arr, n);

    shuffle_arr(arr, n);
    insertion_sort(arr, n);
    printf("Insertion sort :\n");
    print_arr(arr, n);

    shuffle_arr(arr, n);
    quick_sort(arr, 0, n - 1);
    printf("Quick sort :\n");
    print_arr(arr, n);

    shuffle_arr(arr, n);
    merge_sort(arr, n);
    printf("Merge sort :\n");
    print_arr(arr, n);*/

    return 0;
}