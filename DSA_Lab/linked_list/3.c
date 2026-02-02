// Circular Linked List - Menu driven
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* head = NULL;

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void create() {
    int n, data;
    
    printf("Enter number of nodes to create: ");
    scanf("%d", &n);
    
    if(n <= 0) {
        printf("Invalid number!\n");
        return;
    }
    
    for(int i = 1; i <= n; i++) {
        printf("Enter data for node %d: ", i);
        scanf("%d", &data);
        
        struct Node* newNode = createNode(data);
        
        if(head == NULL) {
            head = newNode;
            newNode->next = head;
        } else {
            struct Node* temp = head;
            while(temp->next != head) {
                temp = temp->next;
            }
            temp->next = newNode;
            newNode->next = head;
        }
    }
    printf("Circular linked list created with %d nodes\n", n);
}

void insertAtFront() {
    int data;
    printf("Enter data: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(head == NULL) {
        head = newNode;
        newNode->next = head;
    } else {
        struct Node* temp = head;
        while(temp->next != head) {
            temp = temp->next;
        }
        newNode->next = head;
        temp->next = newNode;
        head = newNode;
    }
    printf("%d inserted at front\n", data);
}

void insertAtEnd() {
    int data;
    printf("Enter data: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    
    if(head == NULL) {
        head = newNode;
        newNode->next = head;
    } else {
        struct Node* temp = head;
        while(temp->next != head) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = head;
    }
    printf("%d inserted at end\n", data);
}

void insertAtPosition() {
    int data, pos;
    printf("Enter position: ");
    scanf("%d", &pos);
    
    if(pos == 1) {
        insertAtFront();
        return;
    }
    
    printf("Enter data: ");
    scanf("%d", &data);
    
    struct Node* newNode = createNode(data);
    struct Node* temp = head;
    
    for(int i = 1; i < pos - 1 && temp->next != head; i++) {
        temp = temp->next;
    }
    
    newNode->next = temp->next;
    temp->next = newNode;
    printf("%d inserted at position %d\n", data, pos);
}

void deleteFromFront() {
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    if(head->next == head) {
        printf("%d deleted from front\n", head->data);
        free(head);
        head = NULL;
        return;
    }
    
    struct Node* temp = head;
    struct Node* last = head;
    
    while(last->next != head) {
        last = last->next;
    }
    
    head = head->next;
    last->next = head;
    printf("%d deleted from front\n", temp->data);
    free(temp);
}

void deleteFromEnd() {
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    if(head->next == head) {
        printf("%d deleted from end\n", head->data);
        free(head);
        head = NULL;
        return;
    }
    
    struct Node* temp = head;
    while(temp->next->next != head) {
        temp = temp->next;
    }
    
    struct Node* last = temp->next;
    temp->next = head;
    printf("%d deleted from end\n", last->data);
    free(last);
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
    for(int i = 1; i < pos - 1 && temp->next != head; i++) {
        temp = temp->next;
    }
    
    if(temp->next == head) {
        printf("Invalid position!\n");
        return;
    }
    
    struct Node* nodeToDelete = temp->next;
    temp->next = nodeToDelete->next;
    printf("%d deleted from position %d\n", nodeToDelete->data, pos);
    free(nodeToDelete);
}

void display() {
    if(head == NULL) {
        printf("List is empty!\n");
        return;
    }
    
    printf("List: ");
    struct Node* temp = head;
    do {
        printf("%d -> ", temp->data);
        temp = temp->next;
    } while(temp != head);
    printf("(back to head)\n");
}

int main() {
    int choice;
    
    while(1) {
        printf("\n--- Circular Linked List Menu ---\n");
        printf("1. Create List\n");
        printf("2. Insert at Front\n");
        printf("3. Insert at End\n");
        printf("4. Insert at Position\n");
        printf("5. Delete from Front\n");
        printf("6. Delete from End\n");
        printf("7. Delete from Position\n");
        printf("8. Display List\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: create(); break;
            case 2: insertAtFront(); break;
            case 3: insertAtEnd(); break;
            case 4: insertAtPosition(); break;
            case 5: deleteFromFront(); break;
            case 6: deleteFromEnd(); break;
            case 7: deleteFromPosition(); break;
            case 8: display(); break;
            case 9:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
