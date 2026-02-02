// Linear Queue using Linked List - Menu driven
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* front = NULL;
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
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    printf("%d inserted into queue\n", data);
}

void dequeue() {
    if(front == NULL) {
        printf("Queue Underflow!\n");
        return;
    }
    
    struct Node* temp = front;
    printf("%d deleted from queue\n", front->data);
    front = front->next;
    
    if(front == NULL) {
        rear = NULL;
    }
    
    free(temp);
}

void display() {
    if(front == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    
    printf("Queue elements: ");
    struct Node* temp = front;
    while(temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void peek() {
    if(front == NULL) {
        printf("Queue is empty!\n");
        return;
    }
    printf("Front element: %d\n", front->data);
}

int isEmpty() {
    return front == NULL;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Linear Queue using Linked List Menu ---\n");
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
