// Dequeue using Linked List - Menu driven
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* front = NULL;
struct Node* rear = NULL;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertFront() {
    int data;
    printf("Enter data to insert at front: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(front == NULL) {
        front = rear = newNode;
    } else {
        newNode->next = front;
        front->prev = newNode;
        front = newNode;
    }
    printf("%d inserted at front\n", data);
}

void insertRear() {
    int data;
    printf("Enter data to insert at rear: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        newNode->prev = rear;
        rear = newNode;
    }
    printf("%d inserted at rear\n", data);
}

void deleteFront() {
    if(front == NULL) {
        printf("Dequeue is empty!\n");
        return;
    }
    
    struct Node* temp = front;
    printf("%d deleted from front\n", front->data);
    
    front = front->next;
    
    if(front == NULL) {
        rear = NULL;
    } else {
        front->prev = NULL;
    }
    
    free(temp);
}

void deleteRear() {
    if(rear == NULL) {
        printf("Dequeue is empty!\n");
        return;
    }
    
    struct Node* temp = rear;
    printf("%d deleted from rear\n", rear->data);
    
    rear = rear->prev;
    
    if(rear == NULL) {
        front = NULL;
    } else {
        rear->next = NULL;
    }
    
    free(temp);
}

void display() {
    if(front == NULL) {
        printf("Dequeue is empty!\n");
        return;
    }
    
    printf("Dequeue elements: ");
    struct Node* temp = front;
    while(temp != NULL) {
        printf("%d ", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

int isEmpty() {
    return front == NULL;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Dequeue using Linked List Menu ---\n");
        printf("1. Insert at Front\n");
        printf("2. Insert at Rear\n");
        printf("3. Delete from Front\n");
        printf("4. Delete from Rear\n");
        printf("5. Display\n");
        printf("6. Check if Empty\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: insertFront(); break;
            case 2: insertRear(); break;
            case 3: deleteFront(); break;
            case 4: deleteRear(); break;
            case 5: display(); break;
            case 6:
                if(isEmpty())
                    printf("Dequeue is empty\n");
                else
                    printf("Dequeue is not empty\n");
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
