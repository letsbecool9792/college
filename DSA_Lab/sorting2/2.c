// Heap Sort
#include <stdio.h>

void displayArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void heapify(int arr[], int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    // If left child is larger than root
    if(left < n && arr[left] > arr[largest])
        largest = left;
    
    // If right child is larger than largest so far
    if(right < n && arr[right] > arr[largest])
        largest = right;
    
    // If largest is not root
    if(largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;
        
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void buildHeap(int arr[], int n) {
    // Build max heap (rearrange array)
    for(int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void heapSort(int arr[], int n) {
    printf("\nHeap Sort:\n");
    printf("Initial array: ");
    displayArray(arr, n);
    
    // Build max heap
    buildHeap(arr, n);
    
    printf("\nAfter building max heap: ");
    displayArray(arr, n);
    
    printf("\nSorting process:\n");
    
    // One by one extract elements from heap
    for(int i = n - 1; i > 0; i--) {
        // Move current root to end
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;
        
        printf("Pass %d: ", n - i);
        displayArray(arr, n);
        
        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }
    
    printf("\nFinal sorted array: ");
    displayArray(arr, n);
}

int main() {
    int arr[100], n;
    
    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    heapSort(arr, n);
    
    return 0;
}
