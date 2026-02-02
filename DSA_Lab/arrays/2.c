// Sparse Matrix Representation
#include <stdio.h>

void inputMatrix(int mat[][10], int rows, int cols) {
    printf("Enter elements of the matrix (%d x %d):\n", rows, cols);
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            scanf("%d", &mat[i][j]);
        }
    }
}

void displayMatrix(int mat[][10], int rows, int cols) {
    printf("\nOriginal Matrix:\n");
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}

int createSparseMatrix(int mat[][10], int rows, int cols, int sparse[][3]) {
    int k = 1;
    int nonZeroCount = 0;
    
    // Count non-zero elements
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(mat[i][j] != 0) {
                nonZeroCount++;
            }
        }
    }
    
    // First row contains dimensions and count
    sparse[0][0] = rows;
    sparse[0][1] = cols;
    sparse[0][2] = nonZeroCount;
    
    // Store non-zero elements
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(mat[i][j] != 0) {
                sparse[k][0] = i;
                sparse[k][1] = j;
                sparse[k][2] = mat[i][j];
                k++;
            }
        }
    }
    
    return k;
}

void displaySparseMatrix(int sparse[][3], int size) {
    printf("\nSparse Matrix Representation:\n");
    printf("Row\tCol\tValue\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t%d\t%d\n", sparse[i][0], sparse[i][1], sparse[i][2]);
    }
}

int main() {
    int mat[10][10], sparse[100][3];
    int rows, cols;
    
    printf("Enter number of rows and columns: ");
    scanf("%d %d", &rows, &cols);
    
    inputMatrix(mat, rows, cols);
    displayMatrix(mat, rows, cols);
    
    int size = createSparseMatrix(mat, rows, cols, sparse);
    displaySparseMatrix(sparse, size);
    
    printf("\nTotal non-zero elements: %d\n", sparse[0][2]);
    printf("Space saved: %d elements\n", (rows * cols) - sparse[0][2]);
    
    return 0;
}
