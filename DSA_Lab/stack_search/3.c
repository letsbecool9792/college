// Interpolation Search
#include <stdio.h>

void displayArray(int arr[], int n) {
    printf("Array: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int interpolationSearch(int arr[], int n, int key) {
    int low = 0, high = n - 1;
    
    while(low <= high && key >= arr[low] && key <= arr[high]) {
        // If only one element
        if(low == high) {
            if(arr[low] == key)
                return low;
            return -1;
        }
        
        // Probing the position with keeping uniform distribution in mind
        int pos = low + ((double)(high - low) / (arr[high] - arr[low])) * (key - arr[low]);
        
        // Boundary check
        if(pos < low || pos > high)
            return -1;
        
        // Target found
        if(arr[pos] == key)
            return pos;
        
        // If key is larger, key is in upper part
        if(arr[pos] < key)
            low = pos + 1;
        
        // If key is smaller, key is in lower part
        else
            high = pos - 1;
    }
    
    return -1;
}

void sortArray(int arr[], int n) {
    // Bubble sort
    for(int i = 0; i < n - 1; i++) {
        for(int j = 0; j < n - i - 1; j++) {
            if(arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int arr[100], n, key, result;
    
    printf("--- Interpolation Search ---\n\n");
    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    // Sort the array (required for interpolation search)
    sortArray(arr, n);
    
    printf("\nSorted ");
    displayArray(arr, n);
    
    printf("\nEnter element to search: ");
    scanf("%d", &key);
    
    result = interpolationSearch(arr, n, key);
    
    if(result != -1) {
        printf("\n%d found at index %d (position %d)\n", key, result, result + 1);
    } else {
        printf("\n%d not found in the array\n", key);
    }
    
    printf("\nNote: Interpolation search works best on uniformly distributed sorted data.\n");
    printf("It is more efficient than binary search for large, uniformly distributed datasets.\n");
    
    return 0;
}
