// Binary Search Tree Operations - Count nodes, Count leaves, Height, Delete node
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
    
    return root;
}

int countNodes(struct Node* root) {
    if(root == NULL)
        return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(struct Node* root) {
    if(root == NULL)
        return 0;
    
    if(root->left == NULL && root->right == NULL)
        return 1;
    
    return countLeaves(root->left) + countLeaves(root->right);
}

int findHeight(struct Node* root) {
    if(root == NULL)
        return -1;
    
    int leftHeight = findHeight(root->left);
    int rightHeight = findHeight(root->right);
    
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

struct Node* findMin(struct Node* root) {
    while(root->left != NULL)
        root = root->left;
    return root;
}

struct Node* deleteNode(struct Node* root, int data) {
    if(root == NULL) {
        printf("Node with value %d not found\n", data);
        return root;
    }
    
    if(data < root->data) {
        root->left = deleteNode(root->left, data);
    }
    else if(data > root->data) {
        root->right = deleteNode(root->right, data);
    }
    else {
        // Node with only one child or no child
        if(root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            printf("Node %d deleted successfully\n", data);
            return temp;
        }
        else if(root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            printf("Node %d deleted successfully\n", data);
            return temp;
        }
        
        // Node with two children: Get inorder successor (smallest in right subtree)
        struct Node* temp = findMin(root->right);
        
        // Copy the inorder successor's content to this node
        root->data = temp->data;
        
        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->data);
        printf("Node %d deleted successfully\n", data);
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

int main() {
    struct Node* root = NULL;
    int choice, value, n;
    
    printf("--- Binary Search Tree Operations ---\n\n");
    
    printf("Enter number of nodes to create BST: ");
    scanf("%d", &n);
    
    printf("Enter %d values:\n", n);
    for(int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }
    
    while(1) {
        printf("\n--- BST Operations Menu ---\n");
        printf("1. Count number of nodes\n");
        printf("2. Count number of leaves\n");
        printf("3. Find height of tree\n");
        printf("4. Delete a node\n");
        printf("5. Insert a node\n");
        printf("6. Display tree (Inorder)\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                printf("Total number of nodes: %d\n", countNodes(root));
                break;
                
            case 2:
                printf("Number of leaf nodes: %d\n", countLeaves(root));
                break;
                
            case 3:
                printf("Height of the tree: %d\n", findHeight(root));
                break;
                
            case 4:
                printf("Enter value to delete: ");
                scanf("%d", &value);
                root = deleteNode(root, value);
                break;
                
            case 5:
                printf("Enter value to insert: ");
                scanf("%d", &value);
                root = insert(root, value);
                printf("%d inserted successfully\n", value);
                break;
                
            case 6:
                printf("Inorder Traversal: ");
                inorder(root);
                printf("\n");
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
