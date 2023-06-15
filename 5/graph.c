#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef enum {
    MALE,
    FEMALE
} Gender;

typedef struct {
    char* name;
    Gender gender;
    int birthYear, deathYear;
} PersonInfo;

typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

typedef struct {
    AdjListNode* head;
    PersonInfo info;
} Vertex;

typedef struct {
    int numVertices;
    Vertex* vertices;
} GenealogyList;

AdjListNode* newAdjListNode(int dest) {
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

void addVertex(GenealogyList* list, PersonInfo p) {
    Vertex v;
    v.info = p;
    v.head = NULL;
    list->vertices[list->numVertices++] = v;
}

void addEdge(GenealogyList* list, int v1, int v2) {
    AdjListNode* newNode = newAdjListNode(v2);
    newNode->next = list->vertices[v1].head;
    list->vertices[v1].head = newNode;

    newNode = newAdjListNode(v1);
    newNode->next = list->vertices[v2].head;
    list->vertices[v2].head = newNode;
}

void removeVertex(GenealogyList* list, int v) {
    AdjListNode *curr, *prev;
    for (int i = 0; i < list->numVertices; i++) {
        if (i == v) {
            continue;
        }
        curr = prev = list->vertices[i].head;
        while (curr != NULL) {
            if (curr->dest == v) {
                if (curr == prev) {
                    list->vertices[i].head = curr->next;
                } else {
                    prev->next = curr->next;
                }
                free(curr);
                break;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    for (int i = v; i < list->numVertices - 1; i++) {
        list->vertices[i] = list->vertices[i + 1];
    }
    list->numVertices--;
}

void removeEdge(GenealogyList* list, int v1, int v2) {
    AdjListNode *curr, *prev;
    curr = prev = list->vertices[v1].head;
    while (curr != NULL) {
        if (curr->dest == v2) {
            if (curr == prev) {
                list->vertices[v1].head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    curr = prev = list->vertices[v2].head;
    while (curr != NULL) {
        if (curr->dest == v1) {
            if (curr == prev) {
                list->vertices[v2].head = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}

void DFS(GenealogyList* list, int startVertex) {
    bool visited[list->numVertices];
    memset(visited, false, list->numVertices * sizeof(bool));
    DFSHelper(list, startVertex, visited);
}

void DFSHelper(GenealogyList* list, int vertex, bool visited[]) {
    visited[vertex] = true;
    printf("%s ", list->vertices[vertex].info.name);
    AdjListNode* curr = list->vertices[vertex].head;
    while (curr != NULL) {
        int dest = curr->dest;
        if (!visited[dest]) {
            DFSHelper(list, dest, visited);
        }
        curr = curr->next;
    }
}

void bellmanFord(GenealogyList* list, int startVertex) {
    int dist[list->numVertices];
    for (int i = 0; i < list->numVertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[startVertex] = 0;

    for (int i = 0; i < list->numVertices - 1; i++) {
        for (int j = 0; j < list->numVertices; j++) {
            AdjListNode* curr = list->vertices[j].head;
            while (curr != NULL) {
                int k = curr->dest;
                int weight = abs(list->vertices[j].info.birthYear - list->vertices[k].info.birthYear);
                if (dist[j] != INT_MAX && dist[j] + weight < dist[k]) {
                    dist[k] = dist[j] + weight;
                }
                curr = curr->next;
            }
        }
    }

    printf("Shortest distances from %s:\n", list->vertices[startVertex].info.name);
    for (int i = 0; i < list->numVertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("%s: unreachable\n", list->vertices[i].info.name);
        } else {
            printf("%s: %d\n", list->vertices[i].info.name, dist[i]);
        }
    }
}

void floydWarshall(GenealogyList* list) {
    int dist[list->numVertices][list->numVertices];
    for (int i = 0; i < list->numVertices; i++) {
        for (int j = 0; j < list->numVertices; j++) {
            dist[i][j] = INT_MAX;
        }
    }
    for (int i = 0; i < list->numVertices; i++) {
        dist[i][i] = 0;
        AdjListNode* curr = list->vertices[i].head;
        while (curr != NULL) {
            int j = curr->dest;
            int weight = abs(list->vertices[i].info.birthYear - list->vertices[j].info.birthYear);
            dist[i][j] = dist[j][i] = weight;
            curr = curr->next;
        }
    }

    for (int k = 0; k < list->numVertices; k++) {
        for (int i = 0; i < list->numVertices; i++) {
            for (int j = 0; j < list->numVertices; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printf("Shortest distances:\n");
    for (int i = 0; i < list->numVertices; i++) {
        for (int j = i + 1; j < list->numVertices; j++) {
            if (dist[i][j] == INT_MAX) {
                printf("%s and %s: unreachable\n", list->vertices[i].info.name, list->vertices[j].info.name);
            } else {
                printf("%s and %s: %d\n", list->vertices[i].info.name, list->vertices[j].info.name, dist[i][j]);
            }
        }
    }
}


void printMenu();

void printAdjMatrix(GenealogyList* list);


int main() {
    GenealogyList list;
    list.numVertices = 0;
    list.vertices = (PersonInfo*) malloc(10 * sizeof(PersonInfo));
    int choice;
    do {
        printMenu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
{
    char name[50];
    Gender gender;
    int birthYear, deathYear;
    printf("Enter a person's name: ");
    getchar(); // считываем символ новой строки
    fgets(name, 50, stdin);
    printf("Enter their gender (0 for male, 1 for female): ");
    scanf("%d", &gender);
    printf("Enter their birth year: ");
    scanf("%d", &birthYear);
    printf("Enter their death year (0 if still alive): ");
    scanf("%d", &deathYear);
    addVertex(&list, (PersonInfo) { strdup(name), gender, birthYear, deathYear });
    break;
}
            case 2: {
                int v1, v2;
                printf("Enter the indices of the two vertices to add an edge between: ");
                scanf("%d %d", &v1, &v2);
                addEdge(&list, v1, v2);
                break;
            }
            case 3: {
                int v;
                printf("Enter the index of the vertex to remove: ");
                scanf("%d", &v);
                removeVertex(&list, v);
                break;
            }
            case 4: {
                int v1, v2;
                printf("Enter the indices of the two vertices to remove the edge between: ");
                scanf("%d %d", &v1, &v2);
                removeEdge(&list, v1, v2);
                break;
            }
            case 5: {
                int startVertex;
                printf("Enter the index of the starting vertex for DFS: ");
                scanf("%d", &startVertex);
                DFS(&list, startVertex);
                break;
            }
            case 6: {
                int startVertex;
                printf("Enter the index of the starting vertex for Bellman-Ford: ");
                scanf("%d", &startVertex);
                bellmanFord(&list, startVertex);
                break;
            }
            case 7: {
                floydWarshall(&list);
                break;
            }
            case 8: {
                printAdjMatrix(&list);
                break;
            }
            case 9: {
                printf("Goodbye!\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 9);
    return 0;
}

void printMenu() {
    printf("\n---Genealogy Program---\n");
    printf("1. Add a vertex\n");
    printf("2. Add an edge\n");
    printf("3. Remove a vertex\n");
    printf("4. Remove an edge\n");
    printf("5. Depth-first search\n");
    printf("6. Bellman-Ford algorithm\n");
    printf("7. Floyd-Warshall algorithm\n");
    printf("8. Print adjacency matrix\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

void printAdjMatrix(GenealogyList* list) {
    int numRows = list->numVertices;
    int numCols = list->numVertices;
    int adjMatrix[numRows][numCols];
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            adjMatrix[i][j] = 0;
        }
    }
    for (int i = 0; i < numRows; i++) {
        AdjListNode* curr = list->vertices[i].head;
        while (curr != NULL) {
            adjMatrix[i][curr->dest] = 1;
            curr = curr->next;
        }
    }
    printf("\nAdjacency Matrix:\n");
    printf("  ");
    for (int i = 0; i < numCols; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (int i = 0; i < numRows; i++) {
        printf("%d ", i);
        for (int j = 0; j < numCols; j++) {
            printf("%d ", adjMatrix[i][j]);
        }
        printf("\n");
    }
}
