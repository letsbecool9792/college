// Polynomial Multiplication using Array
#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 100

struct Term {
    int coeff;
    int exp;
};

struct Polynomial {
    struct Term terms[MAX_TERMS];
    int numTerms;
};

void initPolynomial(struct Polynomial* poly) {
    poly->numTerms = 0;
}

void inputPolynomial(struct Polynomial* poly) {
    int n, coeff, exp;
    
    printf("Enter number of terms: ");
    scanf("%d", &n);
    
    if(n > MAX_TERMS) {
        printf("Maximum %d terms allowed!\n", MAX_TERMS);
        n = MAX_TERMS;
    }
    
    printf("Enter terms (coefficient and exponent):\n");
    for(int i = 0; i < n; i++) {
        printf("Term %d - Coefficient: ", i + 1);
        scanf("%d", &coeff);
        printf("Term %d - Exponent: ", i + 1);
        scanf("%d", &exp);
        
        poly->terms[i].coeff = coeff;
        poly->terms[i].exp = exp;
    }
    
    poly->numTerms = n;
}

void displayPolynomial(struct Polynomial* poly) {
    if(poly->numTerms == 0) {
        printf("0");
        return;
    }
    
    int first = 1;
    
    for(int i = 0; i < poly->numTerms; i++) {
        int coeff = poly->terms[i].coeff;
        int exp = poly->terms[i].exp;
        
        if(coeff != 0) {
            if(!first && coeff > 0)
                printf(" + ");
            else if(coeff < 0)
                printf(" - ");
            else if(first && coeff < 0)
                printf("-");
            
            int absCoeff = abs(coeff);
            
            if(absCoeff != 1 || exp == 0)
                printf("%d", absCoeff);
            
            if(exp > 0) {
                printf("x");
                if(exp > 1)
                    printf("^%d", exp);
            }
            
            first = 0;
        }
    }
    printf("\n");
}

void addOrUpdateTerm(struct Polynomial* poly, int coeff, int exp) {
    // Check if term with same exponent already exists
    for(int i = 0; i < poly->numTerms; i++) {
        if(poly->terms[i].exp == exp) {
            poly->terms[i].coeff += coeff;
            return;
        }
    }
    
    // If not found, add new term
    if(poly->numTerms < MAX_TERMS) {
        poly->terms[poly->numTerms].coeff = coeff;
        poly->terms[poly->numTerms].exp = exp;
        poly->numTerms++;
    }
}

void sortPolynomial(struct Polynomial* poly) {
    // Sort by exponent in descending order
    for(int i = 0; i < poly->numTerms - 1; i++) {
        for(int j = 0; j < poly->numTerms - i - 1; j++) {
            if(poly->terms[j].exp < poly->terms[j + 1].exp) {
                // Swap
                struct Term temp = poly->terms[j];
                poly->terms[j] = poly->terms[j + 1];
                poly->terms[j + 1] = temp;
            }
        }
    }
}

void multiplyPolynomials(struct Polynomial* poly1, struct Polynomial* poly2, struct Polynomial* result) {
    initPolynomial(result);
    
    // Multiply each term of poly1 with each term of poly2
    for(int i = 0; i < poly1->numTerms; i++) {
        for(int j = 0; j < poly2->numTerms; j++) {
            int coeffProduct = poly1->terms[i].coeff * poly2->terms[j].coeff;
            int expSum = poly1->terms[i].exp + poly2->terms[j].exp;
            
            addOrUpdateTerm(result, coeffProduct, expSum);
        }
    }
    
    // Sort result polynomial by exponent in descending order
    sortPolynomial(result);
}

int main() {
    struct Polynomial poly1, poly2, result;
    
    printf("--- Polynomial Multiplication using Arrays ---\n\n");
    
    printf("Enter first polynomial:\n");
    initPolynomial(&poly1);
    inputPolynomial(&poly1);
    
    printf("\nEnter second polynomial:\n");
    initPolynomial(&poly2);
    inputPolynomial(&poly2);
    
    printf("\nFirst Polynomial: ");
    displayPolynomial(&poly1);
    
    printf("Second Polynomial: ");
    displayPolynomial(&poly2);
    
    initPolynomial(&result);
    multiplyPolynomials(&poly1, &poly2, &result);
    
    printf("\nProduct of Polynomials: ");
    displayPolynomial(&result);
    
    return 0;
}
