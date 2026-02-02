// Sorting Algorithms: Bubble, Selection, and Insertion Sort
#include <stdio.h>

void displayArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void bubbleSort(int arr[], int n) {
    int swapped;
    
    printf("\nBubble Sort (Optimized):\n");
    printf("Initial array: ");
    displayArray(arr, n);
    
    for(int i = 0; i < n - 1; i++) {
        swapped = 0;
        
        for(int j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                // Swap
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        
        printf("Pass %d: ", i + 1);
        displayArray(arr, n);
        
        // If no swap occurred, array is sorted
        if(swapped == 0) {
            printf("Array is already sorted. Optimization: stopped early!\n");
            break;
        }
    }
    
    printf("Sorted array: ");
    displayArray(arr, n);
}

void selectionSort(int arr[], int n) {
    printf("\nSelection Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    
    for(int i = 0; i < n - 1; i++) {
        int minIndex = i;
        
        // Find minimum element in unsorted array
        for(int j = i + 1; j < n; j++) {
            if(arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        
        // Swap minimum element with first element
        if(minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
        
        printf("Pass %d: ", i + 1);
        displayArray(arr, n);
    }
    
    printf("Sorted array: ");
    displayArray(arr, n);
}

void insertionSort(int arr[], int n) {
    printf("\nInsertion Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    
    for(int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        
        // Move elements greater than key one position ahead
        while(j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        
        arr[j + 1] = key;
        
        printf("Pass %d: ", i);
        displayArray(arr, n);
    }
    
    printf("Sorted array: ");
    displayArray(arr, n);
}

void copyArray(int source[], int dest[], int n) {
    for(int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

int main() {
    int arr[100], temp[100], n, choice;
    
    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    while(1) {
        printf("\n--- Sorting Algorithms Menu ---\n");
        printf("1. Bubble Sort (Optimized)\n");
        printf("2. Selection Sort\n");
        printf("3. Insertion Sort\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        copyArray(arr, temp, n);
        
        switch(choice) {
            case 1:
                bubbleSort(temp, n);
                break;
            case 2:
                selectionSort(temp, n);
                break;
            case 3:
                insertionSort(temp, n);
                break;
            case 4:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
