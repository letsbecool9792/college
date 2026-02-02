// Infix to Postfix Conversion
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

char stack[MAX];
int top = -1;

void push(char ch) {
    if(top == MAX - 1) {
        printf("Stack Overflow!\n");
        return;
    }
    stack[++top] = ch;
}

char pop() {
    if(top == -1) {
        return '\0';
    }
    return stack[top--];
}

char peek() {
    if(top == -1) {
        return '\0';
    }
    return stack[top];
}

int isEmpty() {
    return top == -1;
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^');
}

int precedence(char ch) {
    switch(ch) {
        case '^': return 3;
        case '*':
        case '/':
        case '%': return 2;
        case '+':
        case '-': return 1;
        default: return 0;
    }
}

int isRightAssociative(char ch) {
    return ch == '^';
}

void infixToPostfix(char* infix, char* postfix) {
    int i, j = 0;
    
    for(i = 0; infix[i] != '\0'; i++) {
        // Skip spaces
        if(infix[i] == ' ')
            continue;
            
        // If operand, add to output
        if(isalnum(infix[i])) {
            postfix[j++] = infix[i];
        }
        // If '(', push to stack
        else if(infix[i] == '(') {
            push(infix[i]);
        }
        // If ')', pop until '('
        else if(infix[i] == ')') {
            while(!isEmpty() && peek() != '(') {
                postfix[j++] = pop();
            }
            if(!isEmpty() && peek() == '(') {
                pop(); // Remove '('
            }
        }
        // If operator
        else if(isOperator(infix[i])) {
            while(!isEmpty() && peek() != '(' && 
                  (precedence(peek()) > precedence(infix[i]) ||
                   (precedence(peek()) == precedence(infix[i]) && !isRightAssociative(infix[i])))) {
                postfix[j++] = pop();
            }
            push(infix[i]);
        }
    }
    
    // Pop remaining operators
    while(!isEmpty()) {
        postfix[j++] = pop();
    }
    
    postfix[j] = '\0';
}

int main() {
    char infix[MAX], postfix[MAX];
    
    printf("Enter an infix expression: ");
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0; // Remove newline
    
    infixToPostfix(infix, postfix);
    
    printf("Infix Expression: %s\n", infix);
    printf("Postfix Expression: %s\n", postfix);
    
    return 0;
}
