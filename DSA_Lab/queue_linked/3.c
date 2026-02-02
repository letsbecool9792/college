// Circular Queue using Circular Linked List - Menu driven
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* rear = NULL;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void enqueue() {
    int data;
    printf("Enter data to insert: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(rear == NULL) {
        rear = newNode;
        rear->next = rear;
    } else {
        newNode->next = rear->next;
        rear->next = newNode;
        rear = newNode;
    }
    printf("%d inserted into circular queue\n", data);
}

void dequeue() {
    if(rear == NULL) {
        printf("Queue Underflow!\n");
        return;
    }
    
    struct Node* front = rear->next;
    
    if(front == rear) {
        printf("%d deleted from circular queue\n", front->data);
        free(front);
        rear = NULL;
    } else {
        printf("%d deleted from circular queue\n", front->data);
        rear->next = front->next;
        free(front);
    }
}

void display() {
    if(rear == NULL) {
        printf("Circular queue is empty!\n");
        return;
    }
    
    printf("Circular queue elements: ");
    struct Node* temp = rear->next;
    
    do {
        printf("%d ", temp->data);
        temp = temp->next;
    } while(temp != rear->next);
    
    printf("\n");
}

void peek() {
    if(rear == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    printf("Front element: %d\n", rear->next->data);
}

int isEmpty() {
    return rear == NULL;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Circular Queue using Circular Linked List Menu ---\n");
        printf("1. Enqueue (Insert)\n");
        printf("2. Dequeue (Delete)\n");
        printf("3. Display\n");
        printf("4. Peek\n");
        printf("5. Check if Empty\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: enqueue(); break;
            case 2: dequeue(); break;
            case 3: display(); break;
            case 4: peek(); break;
            case 5:
                if(isEmpty())
                    printf("Queue is empty\n");
                else
                    printf("Queue is not empty\n");
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
