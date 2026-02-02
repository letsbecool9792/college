// Postfix Expression Evaluation
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

int stack[MAX];
int top = -1;

void push(int value) {
    if(top == MAX - 1) {
        printf("Stack Overflow!\n");
        return;
    }
    stack[++top] = value;
}

int pop() {
    if(top == -1) {
        printf("Stack Underflow!\n");
        return -1;
    }
    return stack[top--];
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '^');
}

int performOperation(int operand1, int operand2, char operator) {
    switch(operator) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': 
            if(operand2 == 0) {
                printf("Error: Division by zero!\n");
                return 0;
            }
            return operand1 / operand2;
        case '%': return operand1 % operand2;
        case '^': {
            int result = 1;
            for(int i = 0; i < operand2; i++) {
                result *= operand1;
            }
            return result;
        }
        default: return 0;
    }
}

int evaluatePostfix(char* expression) {
    int i, operand1, operand2, result;
    
    for(i = 0; expression[i] != '\0'; i++) {
        // Skip spaces
        if(expression[i] == ' ')
            continue;
            
        // If the character is a digit, push it to stack
        if(isdigit(expression[i])) {
            int num = 0;
            while(isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            i--; // Adjust for the loop increment
            push(num);
        }
        // If the character is an operator
        else if(isOperator(expression[i])) {
            operand2 = pop();
            operand1 = pop();
            result = performOperation(operand1, operand2, expression[i]);
            push(result);
        }
    }
    
    return pop();
}

int main() {
    char expression[MAX];
    
    printf("Enter a postfix expression (separate multi-digit numbers and operators with spaces): ");
    fgets(expression, MAX, stdin);
    expression[strcspn(expression, "\n")] = 0; // Remove newline
    
    printf("Postfix Expression: %s\n", expression);
    
    int result = evaluatePostfix(expression);
    
    printf("Result: %d\n", result);
    
    return 0;
}
