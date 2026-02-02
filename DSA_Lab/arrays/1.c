// Matrix Multiplication with possibility check
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

int multiplyMatrices(int mat1[][10], int r1, int c1, int mat2[][10], int r2, int c2, int result[][10]) {
    // Check if multiplication is possible
    if(c1 != r2) {
        printf("Matrix multiplication not possible!\n");
        printf("Number of columns in first matrix (%d) must equal number of rows in second matrix (%d)\n", c1, r2);
        return 0;
    }
    
    // Initialize result matrix
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            result[i][j] = 0;
        }
    }
    
    // Multiply matrices
    for(int i = 0; i < r1; i++) {
        for(int j = 0; j < c2; j++) {
            for(int k = 0; k < c1; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    
    return 1;
}

int main() {
    int mat1[10][10], mat2[10][10], result[10][10];
    int r1, c1, r2, c2;
    
    printf("Enter number of rows and columns for first matrix: ");
    scanf("%d %d", &r1, &c1);
    
    inputMatrix(mat1, r1, c1);
    
    printf("\nEnter number of rows and columns for second matrix: ");
    scanf("%d %d", &r2, &c2);
    
    inputMatrix(mat2, r2, c2);
    
    printf("\nFirst Matrix:\n");
    displayMatrix(mat1, r1, c1);
    
    printf("\nSecond Matrix:\n");
    displayMatrix(mat2, r2, c2);
    
    if(multiplyMatrices(mat1, r1, c1, mat2, r2, c2, result)) {
        printf("\nResultant Matrix:\n");
        displayMatrix(result, r1, c2);
    }
    
    return 0;
}
