// Sorting Algorithms: Shell Sort and Radix Sort
#include <stdio.h>

void displayArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void shellSort(int arr[], int n) {
    printf("\nShell Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    
    // Start with a large gap, then reduce the gap
    for(int gap = n / 2; gap > 0; gap /= 2) {
        printf("\nGap = %d:\n", gap);
        
        // Do a gapped insertion sort
        for(int i = gap; i < n; i++) {
            int temp = arr[i];
            int j;
            
            // Shift earlier gap-sorted elements up until the correct location for arr[i] is found
            for(j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            
            arr[j] = temp;
        }
        
        printf("After gap %d: ", gap);
        displayArray(arr, n);
    }
    
    printf("\nSorted array: ");
    displayArray(arr, n);
}

int getMax(int arr[], int n) {
    int max = arr[0];
    for(int i = 1; i < n; i++) {
        if(arr[i] > max)
            max = arr[i];
    }
    return max;
}

void countingSort(int arr[], int n, int exp) {
    int output[n];
    int count[10] = {0};
    
    // Store count of occurrences
    for(int i = 0; i < n; i++) {
        count[(arr[i] / exp) % 10]++;
    }
    
    // Change count[i] so that count[i] contains actual position
    for(int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }
    
    // Build the output array
    for(int i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }
    
    // Copy the output array to arr[]
    for(int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
    
    printf("After sorting by digit at position %d: ", exp);
    displayArray(arr, n);
}

void radixSort(int arr[], int n) {
    printf("\nRadix Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    
    // Find the maximum number to know number of digits
    int max = getMax(arr, n);
    
    // Do counting sort for every digit
    printf("\nSorting by each digit position:\n");
    for(int exp = 1; max / exp > 0; exp *= 10) {
        countingSort(arr, n, exp);
    }
    
    printf("\nSorted array: ");
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
    
    printf("Enter %d elements (positive integers for Radix Sort):\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    while(1) {
        printf("\n--- Sorting Algorithms Menu ---\n");
        printf("1. Shell Sort\n");
        printf("2. Radix Sort\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        copyArray(arr, temp, n);
        
        switch(choice) {
            case 1:
                shellSort(temp, n);
                break;
            case 2:
                radixSort(temp, n);
                break;
            case 3:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
