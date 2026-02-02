// Breadth First Search (BFS) on a Graph
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int graph[MAX][MAX];
int visited[MAX];
int queue[MAX];
int front = -1, rear = -1;

void enqueue(int vertex) {
    if(front == -1)
        front = 0;
    queue[++rear] = vertex;
}

int dequeue() {
    int vertex = queue[front++];
    if(front > rear)
        front = rear = -1;
    return vertex;
}

int isEmpty() {
    return front == -1;
}

void BFS(int startVertex, int numVertices) {
    // Mark all vertices as not visited
    for(int i = 0; i < numVertices; i++) {
        visited[i] = 0;
    }
    
    // Mark the starting vertex as visited and enqueue it
    visited[startVertex] = 1;
    enqueue(startVertex);
    
    printf("\nBFS Traversal starting from vertex %d:\n", startVertex);
    
    while(!isEmpty()) {
        // Dequeue a vertex and print it
        int currentVertex = dequeue();
        printf("%d ", currentVertex);
        
        // Get all adjacent vertices of the dequeued vertex
        // If an adjacent vertex has not been visited, mark it visited and enqueue it
        for(int i = 0; i < numVertices; i++) {
            if(graph[currentVertex][i] == 1 && !visited[i]) {
                visited[i] = 1;
                enqueue(i);
            }
        }
    }
    printf("\n");
}

void inputGraph(int numVertices, int numEdges) {
    int src, dest;
    
    // Initialize graph
    for(int i = 0; i < numVertices; i++) {
        for(int j = 0; j < numVertices; j++) {
            graph[i][j] = 0;
        }
    }
    
    printf("\nEnter edges (source destination):\n");
    printf("(Vertices are numbered from 0 to %d)\n", numVertices - 1);
    
    for(int i = 0; i < numEdges; i++) {
        printf("Edge %d: ", i + 1);
        scanf("%d %d", &src, &dest);
        
        if(src >= 0 && src < numVertices && dest >= 0 && dest < numVertices) {
            graph[src][dest] = 1;
            graph[dest][src] = 1; // For undirected graph
        } else {
            printf("Invalid vertices! Please enter again.\n");
            i--;
        }
    }
}

void displayGraph(int numVertices) {
    printf("\nAdjacency Matrix:\n");
    printf("   ");
    for(int i = 0; i < numVertices; i++) {
        printf("%2d ", i);
    }
    printf("\n");
    
    for(int i = 0; i < numVertices; i++) {
        printf("%2d ", i);
        for(int j = 0; j < numVertices; j++) {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int numVertices, numEdges, startVertex;
    
    printf("--- Breadth First Search (BFS) ---\n\n");
    
    printf("Enter number of vertices: ");
    scanf("%d", &numVertices);
    
    printf("Enter number of edges: ");
    scanf("%d", &numEdges);
    
    inputGraph(numVertices, numEdges);
    
    displayGraph(numVertices);
    
    printf("\nEnter starting vertex for BFS (0 to %d): ", numVertices - 1);
    scanf("%d", &startVertex);
    
    if(startVertex >= 0 && startVertex < numVertices) {
        BFS(startVertex, numVertices);
    } else {
        printf("Invalid starting vertex!\n");
    }
    
    return 0;
}
