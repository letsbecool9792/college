// Sorting Algorithms: Quick Sort and Merge Sort
#include <stdio.h>

void displayArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Quick Sort Functions
int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    
    for(int j = low; j < high; j++) {
        if(arr[j] < pivot) {
            i++;
            // Swap arr[i] and arr[j]
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    // Swap arr[i+1] and arr[high] (pivot)
    int temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

void quickSortRecursive(int arr[], int low, int high, int n) {
    if(low < high) {
        int pi = partition(arr, low, high);
        
        printf("After partitioning around %d: ", arr[pi]);
        displayArray(arr, n);
        
        quickSortRecursive(arr, low, pi - 1, n);
        quickSortRecursive(arr, pi + 1, high, n);
    }
}

void quickSort(int arr[], int n) {
    printf("\nQuick Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    printf("\n");
    
    quickSortRecursive(arr, 0, n - 1, n);
    
    printf("\nSorted array: ");
    displayArray(arr, n);
}

// Merge Sort Functions
void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    int L[n1], R[n2];
    
    // Copy data to temp arrays
    for(int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for(int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    
    // Merge the temp arrays back
    int i = 0, j = 0, k = left;
    
    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining elements
    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    
    while(j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortRecursive(int arr[], int left, int right, int n) {
    if(left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSortRecursive(arr, left, mid, n);
        mergeSortRecursive(arr, mid + 1, right, n);
        
        merge(arr, left, mid, right);
        
        printf("After merging [%d to %d]: ", left, right);
        displayArray(arr, n);
    }
}

void mergeSort(int arr[], int n) {
    printf("\nMerge Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    printf("\n");
    
    mergeSortRecursive(arr, 0, n - 1, n);
    
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
    
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    while(1) {
        printf("\n--- Sorting Algorithms Menu ---\n");
        printf("1. Quick Sort\n");
        printf("2. Merge Sort\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        copyArray(arr, temp, n);
        
        switch(choice) {
            case 1:
                quickSort(temp, n);
                break;
            case 2:
                mergeSort(temp, n);
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
