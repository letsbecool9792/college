// Doubly Linked List - Menu driven
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
};

struct Node* head = NULL;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertAtFront() {
    int data;
    printf("Enter data: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(head != NULL) {
        newNode->next = head;
        head->prev = newNode;
    }
    head = newNode;
    printf("%d inserted at front\n", data);
}

void insertAtEnd() {
    int data;
    printf("Enter data: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(head == NULL) {
        head = newNode;
    } else {
        struct Node* temp = head;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }
    printf("%d inserted at end\n", data);
}

void insertAtPosition() {
    int data, pos;
    printf("Enter position: ");
    scanf("%d", &pos);
    printf("Enter data: ");
    scanf("%d", &data);
    
    if(pos == 1) {
        insertAtFront();
        return;
    }
    
    struct Node* newNode = createNode(data);
    struct Node* temp = head;
    
    for(int i = 1; i < pos - 1 && temp != NULL; i++) {
        temp = temp->next;
    }
    
    if(temp == NULL) {
        printf("Invalid position!\n");
        free(newNode);
        return;
    }
    
    newNode->next = temp->next;
    newNode->prev = temp;
    
    if(temp->next != NULL) {
        temp->next->prev = newNode;
    }
    temp->next = newNode;
    
    printf("%d inserted at position %d\n", data, pos);
}

void deleteFromFront() {
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    struct Node* temp = head;
    head = head->next;
    
    if(head != NULL) {
        head->prev = NULL;
    }
    
    printf("%d deleted from front\n", temp->data);
    free(temp);
}

void deleteFromEnd() {
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    struct Node* temp = head;
    
    if(temp->next == NULL) {
        printf("%d deleted from end\n", temp->data);
        free(temp);
        head = NULL;
        return;
    }
    
    while(temp->next != NULL) {
        temp = temp->next;
    }
    
    temp->prev->next = NULL;
    printf("%d deleted from end\n", temp->data);
    free(temp);
}

void deleteFromPosition() {
    int pos;
    printf("Enter position to delete: ");
    scanf("%d", &pos);
    
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    if(pos == 1) {
        deleteFromFront();
        return;
    }
    
    struct Node* temp = head;
    for(int i = 1; i < pos && temp != NULL; i++) {
        temp = temp->next;
    }
    
    if(temp == NULL) {
        printf("Invalid position!\n");
        return;
    }
    
    if(temp->prev != NULL) {
        temp->prev->next = temp->next;
    }
    if(temp->next != NULL) {
        temp->next->prev = temp->prev;
    }
    
    printf("%d deleted from position %d\n", temp->data, pos);
    free(temp);
}

void display() {
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    printf("List: NULL <-> ");
    struct Node* temp = head;
    while(temp != NULL) {
        printf("%d <-> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

int countNodes() {
    int count = 0;
    struct Node* temp = head;
    
    while(temp != NULL) {
        count++;
        temp = temp->next;
    }
    
    return count;
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Doubly Linked List Menu ---\n");
        printf("1. Insert at Front\n");
        printf("2. Insert at End\n");
        printf("3. Insert at Position\n");
        printf("4. Delete from Front\n");
        printf("5. Delete from End\n");
        printf("6. Delete from Position\n");
        printf("7. Display List\n");
        printf("8. Count Nodes\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: insertAtFront(); break;
            case 2: insertAtEnd(); break;
            case 3: insertAtPosition(); break;
            case 4: deleteFromFront(); break;
            case 5: deleteFromEnd(); break;
            case 6: deleteFromPosition(); break;
            case 7: display(); break;
            case 8: 
                printf("Number of nodes: %d\n", countNodes());
                break;
            case 9:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
