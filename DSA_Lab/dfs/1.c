// Depth First Search (DFS) on a Graph
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int graph[MAX][MAX];
int visited[MAX];

void DFS(int vertex, int numVertices) {
    // Mark the current vertex as visited and print it
    visited[vertex] = 1;
    printf("%d ", vertex);
    
    // Recur for all adjacent vertices
    for(int i = 0; i < numVertices; i++) {
        if(graph[vertex][i] == 1 && !visited[i]) {
            DFS(i, numVertices);
        }
    }
}

void DFSTraversal(int startVertex, int numVertices) {
    // Mark all vertices as not visited
    for(int i = 0; i < numVertices; i++) {
        visited[i] = 0;
    }
    
    printf("\nDFS Traversal starting from vertex %d:\n", startVertex);
    DFS(startVertex, numVertices);
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
    
    printf("--- Depth First Search (DFS) ---\n\n");
    
    printf("Enter number of vertices: ");
    scanf("%d", &numVertices);
    
    printf("Enter number of edges: ");
    scanf("%d", &numEdges);
    
    inputGraph(numVertices, numEdges);
    
    displayGraph(numVertices);
    
    printf("\nEnter starting vertex for DFS (0 to %d): ", numVertices - 1);
    scanf("%d", &startVertex);
    
    if(startVertex >= 0 && startVertex < numVertices) {
        DFSTraversal(startVertex, numVertices);
    } else {
        printf("Invalid starting vertex!\n");
    }
    
    return 0;
}
