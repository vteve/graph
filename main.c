#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Vertex {
    char *identifier;
    int typeRoom;
    int len;
    struct Vertex *next;
} Vertex;

typedef struct Graph {
    Vertex **vertex;
    int countOfVertex;
    int *visited;
} Graph;

int get_int(int *number) {
    int n, f = -10;
    char symbol;
    do {
        f = 0;
        n = scanf("%u", number);
        if (n < 0) {
            return 0;
        } else if (n == 0) {
            printf("Error! Repeat input, please:\n");
            scanf("%*[^\n]");
        } else if (*number < 0) {
            printf("Error! Repeat input, please:\n");
            scanf("%*[^\n]");
        } else if (n == 1) {
            f = scanf("%c", &symbol);
            while (symbol == ' ' || symbol == '\t') {
                f = scanf("%c", &symbol);
            }
            if (symbol != '\n') {
                scanf("%*[^\n]");
                printf("Error! Repeat input, please:\n");
            }
        }
    } while (n == 0 || *number < 0 || symbol != '\n');
    return 1;
}

char *create_str() {
    int len_str = 0;
    char *stdin_str = malloc(len_str + 2);
    char symbol = ' ';
    while (symbol != '\n') {
        stdin_str = realloc(stdin_str, len_str + 2);
        symbol = getchar();
        if (symbol == EOF) {
            printf("End of working.\n");
            free(stdin_str);
            return NULL;
        } else {
            stdin_str[len_str] = symbol;
            len_str++;
        }
    }
    stdin_str[len_str - 1] = '\0';
    return stdin_str;
}

int existenceRoom(Graph *graph, char *identifier) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifier) == 0) {
            return 0;
        }
    }
    return 1;
}

int addVertex(Graph *graph, char *identifier, int typeRoom) {
    graph->countOfVertex++;
    graph->vertex = realloc(graph->vertex, sizeof(Vertex *) * graph->countOfVertex);
    graph->visited = realloc(graph->visited, sizeof(int) * graph->countOfVertex);
    graph->visited[graph->countOfVertex - 1] = 0;
    graph->vertex[graph->countOfVertex - 1] = malloc(sizeof(Vertex));
    graph->vertex[graph->countOfVertex - 1]->identifier = identifier;
    graph->vertex[graph->countOfVertex - 1]->typeRoom = typeRoom;
    graph->vertex[graph->countOfVertex - 1]->len = 0;
    graph->vertex[graph->countOfVertex - 1]->next = NULL;
    return 0;
}

int dialogueAddVertex(Graph *graph) {
    printf("Enter a identifier of vertex:\n");
    char *identifier;
    int flagForIdentifier;
    do {
        identifier = create_str();
        if (!identifier) {
            free(identifier);
            return 2;
        }
        if ((flagForIdentifier = existenceRoom(graph, identifier)) == 0) {
            free(identifier);
            printf("Graph already contains this identifier. Try again:\n");
        }
    } while (flagForIdentifier == 0);
    printf("Enter a type of the room:\n");
    printf("Enter 1 for entrance to the labyrinth.\n");
    printf("Enter 2 for exit from the labyrinth.\n");
    printf("Enter 3 for passage in the labyrinth.\n");
    int typeRoom;
    do {
        if (get_int(&typeRoom) != 1) {
            printf("Error! Restart program, please.\n");
            free(identifier);
            return 2;
        }
        if (typeRoom != 1 && typeRoom != 2 && typeRoom != 3) {
            printf("You've entered incorrect data. Try again:\n");
        }
    } while (typeRoom != 1 && typeRoom != 2 && typeRoom != 3);
    addVertex(graph, identifier, typeRoom);
    return 0;
}

int addEdge(Graph *graph, char *identifierFrom, char *identifierTo, int len) {
    int index1, index2;
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(identifierFrom, graph->vertex[i]->identifier) == 0) {
            index1 = i;
        }
        if (strcmp(identifierTo, graph->vertex[i]->identifier) == 0) {
            index2 = i;
        }
    }
    Vertex *tmp1 = graph->vertex[index1];
    while (tmp1->next) {
        if (strcmp(tmp1->identifier, identifierTo) == 0) {
            return 1;
        }
        tmp1 = tmp1->next;
    }
    if (strcmp(tmp1->identifier, identifierTo) == 0) {
        return 1;
    }
    Vertex *new_vertex = malloc(sizeof(Vertex));
    new_vertex->identifier = strdup(identifierTo);
    new_vertex->next = NULL;
    new_vertex->typeRoom = graph->vertex[index2]->typeRoom;
    new_vertex->len = len;
    tmp1->next = new_vertex;
    return 0;
}

int dialogueAddEdge(Graph *graph) {
    printf("Enter the vertex from which the edge goes:\n");
    char *identifierFrom;
    int p;
    do {
        identifierFrom = create_str();
        if (!identifierFrom) {
            free(identifierFrom);
            return 2;
        }
        if ((p = existenceRoom(graph, identifierFrom)) == 1) {
            free(identifierFrom);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p == 1);
    printf("Enter the vertex where the edge goes:\n");
    char *identifierTo;
    int p2;
    do {
        identifierTo = create_str();
        if (!identifierTo) {
            free(identifierFrom);
            free(identifierTo);
            return 2;
        }
        if ((p2 = existenceRoom(graph, identifierTo)) == 1) {
            free(identifierTo);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p2 == 1);
    if (strcmp(identifierFrom, identifierTo) == 0) {
        free(identifierFrom);
        free(identifierTo);
        printf("You can't enter the same identifiers.\n");
        return 1;
    }
    printf("Enter the length of this edge:\n");
    int len;
    if (get_int(&len) != 1) {
        printf("Error! Restart program, please.\n");
        free(identifierFrom);
        free(identifierTo);
        return 2;
    }
    if (addEdge(graph, identifierFrom, identifierTo, len) == 1) {
        printf("This edge is already exists.\n");
        free(identifierFrom);
        free(identifierTo);
        return 1;
    }
    free(identifierFrom);
    free(identifierTo);
    return 0;
}

void shiftLeft(Graph *graph, int index) {
    for (int i = index; i < graph->countOfVertex - 1; ++i) {
        graph->vertex[i] = graph->vertex[i + 1];
    }
    graph->countOfVertex--;
}

void deleteVertex(Graph *graph, char *identifier) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifier) == 0) {
            while (graph->vertex[i]) {
                Vertex *toDel = graph->vertex[i];
                graph->vertex[i] = graph->vertex[i]->next;
                free(toDel->identifier);
                free(toDel);
            }
            shiftLeft(graph, i);
        }
        Vertex *prev;
        Vertex *start = graph->vertex[i];
        while (start) {
            if (strcmp(start->identifier, identifier) == 0) {
                Vertex *toDel = start;
                start = start->next;
                prev->next = start;
                free(toDel->identifier);
                free(toDel);
            } else {
                prev = start;
                start = start->next;
            }
        }
    }
}

int dialogueDeleteVertex(Graph *graph) {
    printf("Enter the identifier of vertex would you like to delete:\n");
    char *identifier;
    int p;
    do {
        identifier = create_str();
        if (!identifier) {
            free(identifier);
            return 2;
        }
        if ((p = existenceRoom(graph, identifier)) == 1) {
            free(identifier);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p == 1);
    deleteVertex(graph, identifier);
    free(identifier);
    return 0;
}

int deleteEdge(Graph *graph, char *identifierFrom, char *identifierTo) {
    int index = -1;
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifierFrom) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 1;
    }
    Vertex *prev;
    Vertex *tmp = graph->vertex[index];
    int flag = 0;
    while (tmp) {
        if (strcmp(tmp->identifier, identifierTo) == 0) {
            Vertex *toDel = tmp;
            tmp = tmp->next;
            prev->next = tmp;
            free(toDel->identifier);
            free(toDel);
            flag = 1;
        } else {
            prev = tmp;
            tmp = tmp->next;
        }
    }
    if (flag == 0) {
        return 1;
    }
    return 0;
}

int dialogueDeleteEdge(Graph *graph) {
    printf("Enter the vertex from which the edge goes:\n");
    char *identifierFrom;
    int p;
    do {
        identifierFrom = create_str();
        if (!identifierFrom) {
            free(identifierFrom);
            return 2;
        }
        if ((p = existenceRoom(graph, identifierFrom)) == 1) {
            free(identifierFrom);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p == 1);
    printf("Enter the vertex where the edge goes:\n");
    char *identifierTo;
    int p2;
    do {
        identifierTo = create_str();
        if (!identifierTo) {
            free(identifierFrom);
            free(identifierTo);
            return 2;
        }
        if ((p2 = existenceRoom(graph, identifierTo)) == 1) {
            free(identifierTo);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p2 == 1);
    if (strcmp(identifierFrom, identifierTo) == 0) {
        printf("You can't enter the same identifiers.\n");
        free(identifierFrom);
        free(identifierTo);
        return 1;
    }
    if (deleteEdge(graph, identifierFrom, identifierTo) == 1) {
        free(identifierFrom);
        free(identifierTo);
        printf("No such edge exists.\n");
        return 1;
    }
    free(identifierFrom);
    free(identifierTo);
    return 0;
}

void vertexDataChange(Graph *graph, char *identifier, char *newIdentifier, int typeRoom) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifier) == 0) {
            free(graph->vertex[i]->identifier);
            graph->vertex[i]->identifier = strdup(newIdentifier);
            graph->vertex[i]->typeRoom = typeRoom;
        } else {
            Vertex *tmp = graph->vertex[i];
            while (tmp) {
                if (strcmp(tmp->identifier, identifier) == 0) {
                    free(tmp->identifier);
                    tmp->identifier = strdup(newIdentifier);
                    tmp->typeRoom = typeRoom;
                }
                tmp = tmp->next;
            }
        }
    }
}

int dialogueVertexDataChange(Graph *graph) {
    printf("Enter the vertex whose identifier you would like to change:\n");
    char *identifier;
    int flagForIdentifier;
    do {
        identifier = create_str();
        if (!identifier) {
            free(identifier);
            return 2;
        }
        if ((flagForIdentifier = existenceRoom(graph, identifier)) == 1) {
            free(identifier);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (flagForIdentifier == 1);
    printf("Enter new identifier:\n");
    char *newIdentifier;
    int flagForIdentifier2;
    do {
        newIdentifier = create_str();
        if (!newIdentifier) {
            free(identifier);
            free(newIdentifier);
            return 2;
        }
        if ((flagForIdentifier2 = existenceRoom(graph, newIdentifier)) == 0) {
            free(newIdentifier);
            printf("Graph already contains this identifier. Try again:\n");
        }
    } while (flagForIdentifier2 == 0);
    printf("Enter new type of the room:\n");
    printf("Enter 1 for entrance to the labyrinth.\n");
    printf("Enter 2 for exit from the labyrinth.\n");
    printf("Enter 3 for passage in the labyrinth.\n");
    int typeRoom;
    do {
        if (get_int(&typeRoom) != 1) {
            printf("Error! Restart program, please.\n");
            free(identifier);
            free(newIdentifier);
            return 2;
        }
        if (typeRoom != 1 && typeRoom != 2 && typeRoom != 3) {
            printf("You've entered incorrect data. Try again:\n");
        }
    } while (typeRoom != 1 && typeRoom != 2 && typeRoom != 3);
    vertexDataChange(graph, identifier, newIdentifier, typeRoom);
    free(identifier);
    free(newIdentifier);
    return 0;
}

int edgeDataChange(Graph *graph, char *identifierFrom, char *identifierTo, int len) {
    int index = -1;
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifierFrom) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        return 1;
    }
    Vertex *tmp = graph->vertex[index];
    int flag = 0;
    while (tmp) {
        if (strcmp(tmp->identifier, identifierTo) == 0) {
            tmp->len = len;
            flag = 1;
            break;
        }
        tmp = tmp->next;
    }
    if (flag == 0) {
        return 1;
    }
    return 0;
}

int dialogueEdgeDataChange(Graph *graph) {
    printf("Enter the identifier of vertex from which the edge goes:\n");
    char *identifierFrom;
    int p;
    do {
        identifierFrom = create_str();
        if (!identifierFrom) {
            free(identifierFrom);
            return 2;
        }
        if ((p = existenceRoom(graph, identifierFrom)) == 1) {
            free(identifierFrom);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p == 1);
    printf("Enter the identifier of vertex where the edge goes:\n");
    char *identifierTo;
    int p2;
    do {
        identifierTo = create_str();
        if (!identifierTo) {
            free(identifierFrom);
            free(identifierTo);
            return 2;
        }
        if ((p2 = existenceRoom(graph, identifierTo)) == 1) {
            free(identifierTo);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p2 == 1);
    if (strcmp(identifierFrom, identifierTo) == 0) {
        free(identifierFrom);
        free(identifierTo);
        printf("You can't enter the same identifiers.\n");
        return 1;
    }
    printf("Enter new length:\n");
    int len;
    if (get_int(&len) != 1) {
        printf("Error! Restart program, please.\n");
        free(identifierFrom);
        free(identifierTo);
        return 2;
    }
    if (edgeDataChange(graph, identifierFrom, identifierTo, len) == 1) {
        printf("No such edge exists.\n");
        return 1;
    }
    free(identifierFrom);
    free(identifierTo);
    return 0;
}

char *dfs(Graph *graph, int index) {
    Vertex *list = graph->vertex[index];
    Vertex *tmp = list;
    graph->visited[index] = 1;
    char *result;
    if (list->typeRoom == 2) {
        return "You can get out of the maze.\n";
    }
    printf("Visited %s \n", list->identifier);
    while (tmp != NULL) {
        int next_index;
        for (int i = 0; i < graph->countOfVertex; ++i) {
            if (strcmp(graph->vertex[i]->identifier, tmp->identifier) == 0) {
                next_index = i;
            }
        }
        if (graph->visited[next_index] == 0) {
            result = dfs(graph, next_index);
            if (strcmp(result, "You can get out of the maze.\n") == 0) {
                return result;
            }
        }
        tmp = tmp->next;
    }
    return "You can't get out of the maze.\n";
}

int dialogueDfs(Graph *graph) {
    printf("Enter the identifier of vertex from which you will start walking:\n");
    char *identifier;
    int p, index;
    do {
        identifier = create_str();
        if (!identifier) {
            free(identifier);
            return 2;
        }
        if ((p = existenceRoom(graph, identifier)) == 1) {
            free(identifier);
            printf("Graph doesn't contain this identifier. Try again:\n");
        }
    } while (p == 1);
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(identifier, graph->vertex[i]->identifier) == 0) {
            index = i;
        }
    }
    printf("%s", dfs(graph, index));
    for (int i = 0; i < graph->countOfVertex; ++i) {
        graph->visited[i] = 0;
    }
    free(identifier);
    return 0;
}

int entranceOrNot(Graph *graph, char *identifier) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifier) == 0 && graph->vertex[i]->typeRoom != 1) {
            return 1;
        } else if (strcmp(graph->vertex[i]->identifier, identifier) == 0) {
            return 0;
        }
    }
    return 2;
}

int exitOrNot(Graph *graph, char *identifier) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, identifier) == 0 && graph->vertex[i]->typeRoom != 2) {
            return 1;
        } else if (strcmp(graph->vertex[i]->identifier, identifier) == 0) {
            return 0;
        }
    }
    return 2;
}

int indexOfVertex(Graph *graph, char *str) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(graph->vertex[i]->identifier, str) == 0) {
            return i;
        }
    }
    return -1;
}

int dijkstra(Graph *graph, char *identifier1, char *identifier2) {
    int firstIndex = 0;
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(identifier1, graph->vertex[i]->identifier) == 0) {
            firstIndex = i;
            break;
        }
    }
    int distance[graph->countOfVertex];
    for (int i = 0; i < graph->countOfVertex; ++i) {
        distance[i] = 10000;
    }
    distance[firstIndex] = 0;
    int min, minIndex, temp;
    do {
        minIndex = 10000;
        min = 10000;
        for (int i = 0; i < graph->countOfVertex; i++) {
            if ((graph->visited[i] == 0) && (distance[i] < min)) {
                min = distance[i];
                minIndex = i;
            }
        }
        if (minIndex != 10000) {
            Vertex *tmp = graph->vertex[minIndex];
            while (tmp->next) {
                tmp = tmp->next;
                if (tmp->len > 0) {
                    temp = min + tmp->len;
                    int a;
                    if (temp < distance[a = indexOfVertex(graph, tmp->identifier)]) {
                        distance[a] = temp;
                    }
                }
            }
            graph->visited[minIndex] = 1;
        }
    } while (minIndex < 10000);
    for (int i = 0; i < graph->countOfVertex; ++i) {
        graph->visited[i] = 0;
    }
    int b;
    if (distance[b = indexOfVertex(graph, identifier2)] != 10000)
        return distance[b];
    else {
        return -1;
    }
}


int dialogueDijkstra(Graph *graph) {
    printf("Enter the identifier of vertex which is entrance:\n");
    char *identifier1;
    int p;
    do {
        identifier1 = create_str();
        if (!identifier1) {
            free(identifier1);
            return 2;
        }
        if ((p = entranceOrNot(graph, identifier1)) == 1 || p == 2) {
            free(identifier1);
            printf("Graph doesn't contain vertex with this identifier or this vertex isn't entrance. Try again:\n");
        }
    } while (p == 1 || p == 2);
    printf("Enter the identifier of vertex which is exit:\n");
    char *identifier2;
    int p2;
    do {
        identifier2 = create_str();
        if (!identifier2) {
            free(identifier1);
            free(identifier2);
            return 2;
        }
        if ((p2 = exitOrNot(graph, identifier2)) == 1 || p2 == 2) {
            free(identifier2);
            printf("Graph doesn't contain vertex with this identifier or this vertex isn't exit. Try again:\n");
        }
    } while (p2 == 1 || p2 == 2);
    printf("%d\n", dijkstra(graph, identifier1, identifier2));
    free(identifier1);
    free(identifier2);
    return 0;
}

Vertex *floydWarshall(Graph *graph, char *identifier) {
    int matrix[graph->countOfVertex][graph->countOfVertex];
    int index = 0;
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (strcmp(identifier, graph->vertex[i]->identifier) == 0) {
            index = i;
        }
        Vertex *tmp = graph->vertex[i];
        for (int j = 0; j < graph->countOfVertex; ++j) {
            matrix[i][j] = 100000;
        }
        while (tmp) {
            for (int j = 0; j < graph->countOfVertex; ++j) {
                if (strcmp(tmp->identifier, graph->vertex[j]->identifier) == 0) {
                    matrix[i][j] = tmp->len;
                }
            }
            tmp = tmp->next;
        }
    }
    for (int i = 0; i < graph->countOfVertex; ++i) {
        for (int j = 0; j < graph->countOfVertex; ++j) {
            for (int k = 0; k < graph->countOfVertex; ++k) {
                if (matrix[j][k] > matrix[j][i] + matrix[i][k]) {
                    matrix[j][k] = matrix[j][i] + matrix[i][k];
                }
            }
        }
    }
    int min = 100000, minIndex = 10000;
    for (int i = 0; i < graph->countOfVertex; ++i) {
        if (graph->vertex[i]->typeRoom == 2 && min > matrix[index][i]) {
            min = matrix[index][i];
            minIndex = i;
        }
    }
    if (minIndex != 10000) {
        printf("%s\t%d\n", graph->vertex[minIndex]->identifier, min);
        return graph->vertex[minIndex];
    } else {
        printf("You can't go out from maze.\n");
        return NULL;
    }
}

int dialogueFloydWarshall(Graph *graph) {
    printf("Enter the identifier of vertex which is entrance:\n");
    char *identifier;
    int p;
    do {
        identifier = create_str();
        if (!identifier) {
            free(identifier);
            return 2;
        }
        if ((p = entranceOrNot(graph, identifier)) == 1 || p == 2) {
            free(identifier);
            printf("Graph doesn't contain vertex with this identifier or this vertex isn't entrance. Try again:\n");
        }
    } while (p == 1 || p == 2);
    floydWarshall(graph, identifier);
    free(identifier);
    return 0;
}

void createDotFile(Graph *graph, FILE *file, int index) {
    if (graph == NULL) {
        return;
    }
    Vertex *list = graph->vertex[index];
    Vertex *tmp = list;
    graph->visited[index] = 1;
    fprintf(file, "    %s [label=\"%s\"];\n", list->identifier, list->identifier);
    while (tmp != NULL) {
        int next_index;
        for (int i = 0; i < graph->countOfVertex; ++i) {
            if (strcmp(graph->vertex[i]->identifier, tmp->identifier) == 0) {
                next_index = i;
            }
        }
        if (graph->visited[next_index] == 0) {
            fprintf(file, "    %s -> %s [color=\"gold\", arrowhead=\"vee\"];\n", list->identifier, tmp->identifier);
            createDotFile(graph, file, next_index);
        }
        tmp = tmp->next;
    }
}

int generateDotFile(Graph *graph, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error with opening file.\n");
        return 1;
    }
    fprintf(file, "digraph G {\n");
    fprintf(file, "    node [shape=circle, style=filled, color=lightblue, fontcolor=black];\n");
    createDotFile(graph, file, 0);
    for (int i = 0; i < graph->countOfVertex; ++i) {
        graph->visited[i] = 0;
    }
    fprintf(file, "}\n");
    fclose(file);
    printf("File done.\n");
    return 0;
}

void freeGraph(Graph *graph) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        while (graph->vertex[i]) {
            free(graph->vertex[i]->identifier);
            Vertex *toDel = graph->vertex[i];
            graph->vertex[i] = graph->vertex[i]->next;
            free(toDel);
        }
    }
    free(graph->vertex);
    free(graph->visited);
}

void show(Graph *graph) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        Vertex *to_show = graph->vertex[i];
        while (to_show) {
            printf("%s\t%d\t%d\n", to_show->identifier, to_show->len, to_show->typeRoom);
            to_show = to_show->next;
        }
    }
}

int dialogueShow(Graph *graph) {
    for (int i = 0; i < graph->countOfVertex; ++i) {
        Vertex *toShow = graph->vertex[i];
        while (toShow) {
            printf("%s->", toShow->identifier);
            if (!toShow->next) {
                printf("NULL");
            }
            toShow = toShow->next;
        }
        printf("\n");
    }
    return 0;
}

int dialog(const char *msgs[], int len_msg) {
    int rc;
    for (int i = 0; i < len_msg; ++i) {
        puts(msgs[i]);
    }
    do {
        if (get_int(&rc) != 1) {
            printf("Error! Restart program, please.\n");
            return -1;
        }
    } while (rc < 0 || rc > len_msg - 1);
    printf("%d\n", rc);
    return rc;
}

int main() {
    const char *msg[] = {"0. Quit",
                         "1. Add vertex in the graph",
                         "2. Remove vertex in the graph",
                         "3. Add edge in the graph",
                         "4. Remove edge in the graph",
                         "5. Change data in vertex",
                         "6. Change data in edge",
                         "7. Show like a adjacency list",
                         "8. Checking the reachability of at least one of the exits from the specified entry point",
                         "9. Finding the shortest path between a specified input and a specified output",
                         "10. Finding the nearest exit to the specified entrance and the distance to it"};
    const int len_msg = sizeof(msg) / sizeof(msg[0]);
    Graph graph = {NULL, 0, NULL};
    int (*fptr[])(Graph *) = {NULL, dialogueAddVertex, dialogueDeleteVertex, dialogueAddEdge, dialogueDeleteEdge,
                              dialogueVertexDataChange,
                              dialogueEdgeDataChange, dialogueShow, dialogueDfs, dialogueDijkstra,
                              dialogueFloydWarshall};
    int rc = 142412;
    while (rc != 0) {
        if ((rc = dialog(msg, len_msg)) == -1) {
            break;
        }
        if (rc == 0) {
            break;
        }
        if (fptr[rc](&graph) == 2) {
            break;
        }
    }
    printf("That's all. Bye!\n");
    freeGraph(&graph);
    return 0;
}
