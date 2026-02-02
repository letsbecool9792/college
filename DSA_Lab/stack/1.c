// Stack implementation using array - Menu driven
#include <stdio.h>
#define MAX 100

int stack[MAX];
int top = -1;

void push() {
    int value;
    if(top == MAX - 1) {
        printf("Stack Overflow!\n");
        return;
    }
    printf("Enter value to push: ");
    scanf("%d", &value);
    stack[++top] = value;
    printf("%d pushed to stack\n", value);
}

void pop() {
    if(top == -1) {
        printf("Stack Underflow!\n");
        return;
    }
    printf("%d popped from stack\n", stack[top--]);
}

void peek() {
    if(top == -1) {
        printf("Stack is empty!\n");
        return;
    }
    printf("Top element: %d\n", stack[top]);
}

void display() {
    if(top == -1) {
        printf("Stack is empty!\n");
        return;
    }
    printf("Stack elements:\n");
    for(int i = top; i >= 0; i--) {
        printf("%d\n", stack[i]);
    }
}

int isEmpty() {
    return top == -1;
}

int isFull() {
    return top == MAX - 1;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Stack Operations Menu ---\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Check if Empty\n");
        printf("6. Check if Full\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                push();
                break;
            case 2:
                pop();
                break;
            case 3:
                peek();
                break;
            case 4:
                display();
                break;
            case 5:
                if(isEmpty())
                    printf("Stack is empty\n");
                else
                    printf("Stack is not empty\n");
                break;
            case 6:
                if(isFull())
                    printf("Stack is full\n");
                else
                    printf("Stack is not full\n");
                break;
            case 7:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
