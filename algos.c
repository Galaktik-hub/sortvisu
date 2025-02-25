#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int isSorted(int arr[], size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        if (arr[i] > arr[i + 1])
            return 0;
    }
    return 1;
}

void swap(int* x, int* y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}


void shuffleArr(int arr[], size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() % (n - i);
            swap(&arr[i], &arr[j]);
        }
    }
}

void printArr(int arr[], size_t n) {
    if (n == 0)
        return;
    
    for (size_t i = 0; i < n - 1; i++)
        printf("%d, ", arr[i]);

    printf("%d\n", arr[n - 1]);
}

void bubbleSort(int arr[], size_t n) {
    size_t i, j;
    int swapped;
    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(&arr[j], &arr[j + 1]);
                swapped = 1;
            }
        }

        if (swapped == 0)
            break;
    }
}

void selectionSort(int arr[], size_t n) {
    size_t i, j, min;
    for (i = 0; i <= n - 2; i++) {
        min = i;
        for (j = i + 1; j <= n - 1; j++) {
            if (arr[j] < arr[min])
                min = j;
        }
        if (min != i)
            swap(&arr[min], &arr[i]);
    }
}

void insertionSort(int arr[], size_t n) {
    size_t i, j;
    int x;
    for (i = 1; i < n; i ++) {
        x = arr[i];
        j = i;
        while (j > 0 && arr[j - 1] > x) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = x;
    }
}

int partition(int arr[], int low, int high) {
    int pivot = arr[high];

    int i, j;
    i = low;
    for (j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }

    swap(&arr[i], &arr[high]);
    return i;
}

void quickSort(int arr[], int low, int high) {
    if (low >= high || low < 0)
        return;

    int p = partition(arr, low, high);
    quickSort(arr, low, p - 1);
    quickSort(arr, p + 1, high);
}

void mergeSort(int arr[], size_t n) {
    // TODO : Implémenter le merge sort
}

void heapSort(int arr[], size_t n) {
    // TODO : Implémenter le heap sort
}

int stalinSort(int arr[], size_t n) {
    if (n == 0) 
        return n;
    
    size_t j = 1;
    for (size_t i = 1; i < n; i++) {
        if (arr[i] >= arr[j - 1]) {
            arr[j] = arr[i];
            j++;
        }
    }
    
    for (size_t i = j; i < n; i++) {
        arr[i] = 0;
    }

    return j;
}

void bogoSort(int arr[], size_t n) {
    while(isSorted(arr, n) == 0) {
        shuffleArr(arr, n);
    }
}

int main() {
    srand(time(NULL));

    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t n = sizeof(arr) / sizeof(arr[0]);

    printf("Original arr :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    printf("Shuffled arr :\n");
    printArr(arr, n);

    bubbleSort(arr, n);
    printf("Bubble sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    selectionSort(arr, n);
    printf("Selection sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    insertionSort(arr, n);
    printf("Insertion sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    quickSort(arr, 0, n - 1);
    printf("Quick sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    mergeSort(arr, n);
    printf("Merge sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    heapSort(arr, n);
    printf("Heap sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    //bogoSort(arr, n);
    printf("Bogo sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    n = stalinSort(arr, n);
    printf("Stalin sort :\n");
    printArr(arr, n);

    return 0;
}