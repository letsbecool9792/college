// Linear Search and Binary Search - Menu driven
#include <stdio.h>

void displayArray(int arr[], int n) {
    printf("Array: ");
    for(int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int linearSearch(int arr[], int n, int key) {
    for(int i = 0; i < n; i++) {
        if(arr[i] == key) {
            return i;
        }
    }
    return -1;
}

int binarySearch(int arr[], int n, int key) {
    int left = 0, right = n - 1;
    
    while(left <= right) {
        int mid = left + (right - left) / 2;
        
        if(arr[mid] == key) {
            return mid;
        }
        else if(arr[mid] < key) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    
    return -1;
}

void sortArray(int arr[], int n) {
    // Bubble sort for simplicity
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
    int arr[100], n, choice, key, result;
    
    printf("Enter number of elements: ");
    scanf("%d", &n);
    
    printf("Enter %d elements:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    while(1) {
        printf("\n--- Search Operations Menu ---\n");
        displayArray(arr, n);
        printf("1. Linear Search\n");
        printf("2. Binary Search (requires sorted array)\n");
        printf("3. Sort Array\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Enter element to search: ");
                scanf("%d", &key);
                result = linearSearch(arr, n, key);
                if(result != -1)
                    printf("%d found at index %d (position %d)\n", key, result, result + 1);
                else
                    printf("%d not found in the array\n", key);
                break;
                
            case 2:
                printf("Enter element to search: ");
                scanf("%d", &key);
                printf("Note: Binary search requires sorted array\n");
                result = binarySearch(arr, n, key);
                if(result != -1)
                    printf("%d found at index %d (position %d)\n", key, result, result + 1);
                else
                    printf("%d not found in the array\n", key);
                break;
                
            case 3:
                sortArray(arr, n);
                printf("Array sorted successfully!\n");
                displayArray(arr, n);
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
