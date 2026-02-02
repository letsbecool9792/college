// Binary Search Tree - Creation, Insertion, and Traversals
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* root, int data) {
    if(root == NULL) {
        return createNode(data);
    }
    
    if(data < root->data) {
        root->left = insert(root->left, data);
    }
    else if(data > root->data) {
        root->right = insert(root->right, data);
    }
    else {
        printf("Duplicate value %d not inserted\n", data);
    }
    
    return root;
}

void inorder(struct Node* root) {
    if(root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void preorder(struct Node* root) {
    if(root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct Node* root) {
    if(root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

int main() {
    struct Node* root = NULL;
    int choice, value, n;
    
    printf("--- Binary Search Tree ---\n\n");
    
    printf("Enter number of nodes to create BST: ");
    scanf("%d", &n);
    
    printf("Enter %d values:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }
    
    while(1) {
        printf("\n--- BST Operations Menu ---\n");
        printf("1. Insert a new node\n");
        printf("2. Inorder Traversal (Left-Root-Right)\n");
        printf("3. Preorder Traversal (Root-Left-Right)\n");
        printf("4. Postorder Traversal (Left-Right-Root)\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                root = insert(root, value);
                printf("%d inserted successfully\n", value);
                break;
                
            case 2:
                printf("Inorder Traversal: ");
                inorder(root);
                printf("\n");
                break;
                
            case 3:
                printf("Preorder Traversal: ");
                preorder(root);
                printf("\n");
                break;
                
            case 4:
                printf("Postorder Traversal: ");
                postorder(root);
                printf("\n");
                break;
                
            case 5:
                printf("Exiting...\n");
                return 0;
                
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
