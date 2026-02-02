// Polynomial Representation and Addition using Array
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
    
    printf("Enter terms (coefficient and exponent in descending order of exponent):\n");
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

void addPolynomials(struct Polynomial* poly1, struct Polynomial* poly2, struct Polynomial* result) {
    initPolynomial(result);
    
    int i = 0, j = 0, k = 0;
    
    while(i < poly1->numTerms && j < poly2->numTerms) {
        if(poly1->terms[i].exp > poly2->terms[j].exp) {
            result->terms[k].coeff = poly1->terms[i].coeff;
            result->terms[k].exp = poly1->terms[i].exp;
            i++;
            k++;
        }
        else if(poly1->terms[i].exp < poly2->terms[j].exp) {
            result->terms[k].coeff = poly2->terms[j].coeff;
            result->terms[k].exp = poly2->terms[j].exp;
            j++;
            k++;
        }
        else {
            int sumCoeff = poly1->terms[i].coeff + poly2->terms[j].coeff;
            if(sumCoeff != 0) {
                result->terms[k].coeff = sumCoeff;
                result->terms[k].exp = poly1->terms[i].exp;
                k++;
            }
            i++;
            j++;
        }
    }
    
    // Copy remaining terms from poly1
    while(i < poly1->numTerms) {
        result->terms[k].coeff = poly1->terms[i].coeff;
        result->terms[k].exp = poly1->terms[i].exp;
        i++;
        k++;
    }
    
    // Copy remaining terms from poly2
    while(j < poly2->numTerms) {
        result->terms[k].coeff = poly2->terms[j].coeff;
        result->terms[k].exp = poly2->terms[j].exp;
        j++;
        k++;
    }
    
    result->numTerms = k;
}

int main() {
    struct Polynomial poly1, poly2, result;
    
    printf("--- Polynomial Addition using Arrays ---\n\n");
    
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
    
    addPolynomials(&poly1, &poly2, &result);
    
    printf("\nSum of Polynomials: ");
    displayPolynomial(&result);
    
    return 0;
}
