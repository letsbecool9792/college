// Circular Queue implementation using array - Menu driven
#include <stdio.h>
#define MAX 5

int queue[MAX];
int front = -1, rear = -1;

void enqueue() {
    int value;
    
    // Check if queue is full
    if((rear + 1) % MAX == front) {
        printf("Queue Overflow!\n");
        return;
    }
    
    printf("Enter value to insert: ");
    scanf("%d", &value);
    
    // First element
    if(front == -1) {
        front = 0;
    }
    
    rear = (rear + 1) % MAX;
    queue[rear] = value;
    printf("%d inserted into circular queue\n", value);
}

void dequeue() {
    if(front == -1) {
        printf("Queue Underflow!\n");
        return;
    }
    
    printf("%d deleted from circular queue\n", queue[front]);
    
    // Only one element was present
    if(front == rear) {
        front = rear = -1;
    } else {
        front = (front + 1) % MAX;
    }
}

void display() {
    if(front == -1) {
        printf("Queue is empty!\n");
        return;
    }
    
    printf("Queue elements: ");
    int i = front;
    while(1) {
        printf("%d ", queue[i]);
        if(i == rear)
            break;
        i = (i + 1) % MAX;
    }
    printf("\n");
}

void peek() {
    if(front == -1) {
        printf("Queue is empty!\n");
        return;
    }
    printf("Front element: %d\n", queue[front]);
}

int isEmpty() {
    return front == -1;
}

int isFull() {
    return (rear + 1) % MAX == front;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Circular Queue Operations Menu ---\n");
        printf("1. Enqueue (Insert)\n");
        printf("2. Dequeue (Delete)\n");
        printf("3. Display\n");
        printf("4. Peek (Front element)\n");
        printf("5. Check if Empty\n");
        printf("6. Check if Full\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                enqueue();
                break;
            case 2:
                dequeue();
                break;
            case 3:
                display();
                break;
            case 4:
                peek();
                break;
            case 5:
                if(isEmpty())
                    printf("Queue is empty\n");
                else
                    printf("Queue is not empty\n");
                break;
            case 6:
                if(isFull())
                    printf("Queue is full\n");
                else
                    printf("Queue is not full\n");
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
