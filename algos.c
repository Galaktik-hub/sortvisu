#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*** Defines ***/

#define iLeftChild(i) 2*i + 1

#define iRightChild(i) 2*i + 2

#define iParent(i) floor((i-1) / 2)

/*** Global functions ***/

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

/*** Bubble Sort ***/

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

/*** Selection Sort ***/

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

/*** Insertion Sort ***/

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

/*** Quick Sort ***/

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

/*** Merge Sort ***/

void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

/*** Heap Sort ***/

void siftDown(int arr[], size_t root, size_t end) {
    while (iLeftChild(root) < end) {
        size_t child = iLeftChild(root);
        if (child + 1 < end && arr[child] < arr[child + 1])
            child++;
        
        if (arr[root] < arr[child]) {
            swap(&arr[root], &arr[child]);
            root = child;
        } else {
            return;
        }
    }
}


void heapify(int arr[], size_t count) {
    size_t start = iParent(count - 1) + 1;

    while (start > 0) {
        start--;
        siftDown(arr, start, count);
    }
}

void heapSort(int arr[], size_t count) {
    heapify(arr, count);

    size_t end = count;
    while (end > 0) {
        end--;
        swap(&arr[end], &arr[0]);
        siftDown(arr, 0, end);
    }
}

/*** Stalin Sort ***/

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

/*** Bogo Sort ***/

void bogoSort(int arr[], size_t n) {
    while(isSorted(arr, n) == 0) {
        shuffleArr(arr, n);
    }
}

/*** Main ***/

int main() {
    srand(time(NULL));

    int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, -1};
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
    mergeSort(arr, 0, n - 1);
    printf("Merge sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    heapSort(arr, n);
    printf("Heap sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    bogoSort(arr, n);
    printf("Bogo sort :\n");
    printArr(arr, n);

    shuffleArr(arr, n);
    n = stalinSort(arr, n);
    printf("Stalin sort :\n");
    printArr(arr, n);

    return 0;
}