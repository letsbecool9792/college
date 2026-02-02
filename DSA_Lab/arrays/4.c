// Sparse Matrix and its Transpose
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
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}

int createSparseMatrix(int mat[][10], int rows, int cols, int sparse[][3]) {
    int k = 1;
    
    // Count non-zero elements
    int nonZeroCount = 0;
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

void transposeSparse(int sparse[][3], int transpose[][3], int size) {
    int n = sparse[0][2]; // number of non-zero elements
    
    // Set dimensions for transpose
    transpose[0][0] = sparse[0][1]; // rows = original cols
    transpose[0][1] = sparse[0][0]; // cols = original rows
    transpose[0][2] = sparse[0][2]; // same number of non-zero elements
    
    if(n == 0) return;
    
    int k = 1;
    int cols = sparse[0][1];
    
    // For each column in original matrix
    for(int i = 0; i < cols; i++) {
        // Find elements in that column
        for(int j = 1; j < size; j++) {
            if(sparse[j][1] == i) {
                transpose[k][0] = sparse[j][1]; // swap row and col
                transpose[k][1] = sparse[j][0];
                transpose[k][2] = sparse[j][2]; // value remains same
                k++;
            }
        }
    }
}

void displaySparseMatrix(int sparse[][3], int size) {
    printf("Row\tCol\tValue\n");
    for(int i = 0; i < size; i++) {
        printf("%d\t%d\t%d\n", sparse[i][0], sparse[i][1], sparse[i][2]);
    }
}

int main() {
    int mat[10][10], sparse[100][3], transpose[100][3];
    int rows, cols;
    
    printf("Enter number of rows and columns: ");
    scanf("%d %d", &rows, &cols);
    
    inputMatrix(mat, rows, cols);
    
    printf("\nOriginal Matrix:\n");
    displayMatrix(mat, rows, cols);
    
    int size = createSparseMatrix(mat, rows, cols, sparse);
    
    printf("\nSparse Matrix Representation:\n");
    displaySparseMatrix(sparse, size);
    
    transposeSparse(sparse, transpose, size);
    
    printf("\nTranspose of Sparse Matrix:\n");
    displaySparseMatrix(transpose, size);
    
    return 0;
}
