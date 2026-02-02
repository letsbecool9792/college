// Stack implementation using Linked List - Menu driven
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* top = NULL;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void push() {
    int data;
    printf("Enter data to push: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    newNode->next = top;
    top = newNode;
    printf("%d pushed to stack\n", data);
}

void pop() {
    if(top == NULL) {
        printf("Stack Underflow!\n");
        return;
    }
    
    struct Node* temp = top;
    top = top->next;
    printf("%d popped from stack\n", temp->data);
    free(temp);
}

void peek() {
    if(top == NULL) {
        printf("Stack is empty!\n");
        return;
    }
    printf("Top element: %d\n", top->data);
}

void display() {
    if(top == NULL) {
        printf("Stack is empty!\n");
        return;
    }
    
    printf("Stack elements:\n");
    struct Node* temp = top;
    while(temp != NULL) {
        printf("%d\n", temp->data);
        temp = temp->next;
    }
}

int isEmpty() {
    return top == NULL;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Stack using Linked List Menu ---\n");
        printf("1. Push\n");
        printf("2. Pop\n");
        printf("3. Peek\n");
        printf("4. Display\n");
        printf("5. Check if Empty\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: push(); break;
            case 2: pop(); break;
            case 3: peek(); break;
            case 4: display(); break;
            case 5:
                if(isEmpty())
                    printf("Stack is empty\n");
                else
                    printf("Stack is not empty\n");
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
