// Reverse array elements without using other variable
#include <stdio.h>

void displayArray(int arr[], int n) {
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void reverseArray(int arr[], int n) {
    int start = 0;
    int end = n - 1;
    
    while(start < end) {
        // Swap without using temporary variable (XOR method)
        arr[start] = arr[start] ^ arr[end];
        arr[end] = arr[start] ^ arr[end];
        arr[start] = arr[start] ^ arr[end];
        
        start++;
        end--;
    }
}

int main() {
    int arr[100], n;
    
    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("\nOriginal array: ");
    displayArray(arr, n);
    
    reverseArray(arr, n);
    
    printf("Reversed array: ");
    displayArray(arr, n);
    
    return 0;
}
