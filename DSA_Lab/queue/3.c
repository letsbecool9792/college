// Double-ended Queue (Dequeue) implementation using array - Menu driven
#include <stdio.h>
#define MAX 100

int deque[MAX];
int front = -1, rear = -1;

void insertFront() {
    int value;
    
    // Check overflow
    if((front == 0 && rear == MAX - 1) || (front == rear + 1)) {
        printf("Dequeue Overflow!\n");
        return;
    }
    
    printf("Enter value to insert at front: ");
    scanf("%d", &value);
    
    // First element
    if(front == -1) {
        front = rear = 0;
    }
    // Front at first position, wrap around
    else if(front == 0) {
        front = MAX - 1;
    }
    else {
        front = front - 1;
    }
    
    deque[front] = value;
    printf("%d inserted at front\n", value);
}

void insertRear() {
    int value;
    
    // Check overflow
    if((front == 0 && rear == MAX - 1) || (front == rear + 1)) {
        printf("Dequeue Overflow!\n");
        return;
    }
    
    printf("Enter value to insert at rear: ");
    scanf("%d", &value);
    
    // First element
    if(rear == -1) {
        front = rear = 0;
    }
    // Rear at last position, wrap around
    else if(rear == MAX - 1) {
        rear = 0;
    }
    else {
        rear = rear + 1;
    }
    
    deque[rear] = value;
    printf("%d inserted at rear\n", value);
}

void deleteFront() {
    if(front == -1) {
        printf("Dequeue Underflow!\n");
        return;
    }
    
    printf("%d deleted from front\n", deque[front]);
    
    // Only one element
    if(front == rear) {
        front = rear = -1;
    }
    // Front at last position, wrap around
    else if(front == MAX - 1) {
        front = 0;
    }
    else {
        front = front + 1;
    }
}

void deleteRear() {
    if(rear == -1) {
        printf("Dequeue Underflow!\n");
        return;
    }
    
    printf("%d deleted from rear\n", deque[rear]);
    
    // Only one element
    if(front == rear) {
        front = rear = -1;
    }
    // Rear at first position, wrap around
    else if(rear == 0) {
        rear = MAX - 1;
    }
    else {
        rear = rear - 1;
    }
}

void display() {
    if(front == -1) {
        printf("Dequeue is empty!\n");
        return;
    }
    
    printf("Dequeue elements: ");
    int i = front;
    
    if(front <= rear) {
        for(i = front; i <= rear; i++) {
            printf("%d ", deque[i]);
        }
    } else {
        for(i = front; i < MAX; i++) {
            printf("%d ", deque[i]);
        }
        for(i = 0; i <= rear; i++) {
            printf("%d ", deque[i]);
        }
    }
    printf("\n");
}

int isEmpty() {
    return front == -1;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Double-ended Queue (Dequeue) Operations Menu ---\n");
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
            case 1:
                insertFront();
                break;
            case 2:
                insertRear();
                break;
            case 3:
                deleteFront();
                break;
            case 4:
                deleteRear();
                break;
            case 5:
                display();
                break;
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
