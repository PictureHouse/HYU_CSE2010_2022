#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
    int * key;
    int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
    int size;
    int* node;
    int** matrix;
}Graph;

Graph* CreateGraph(int* nodes, int n);          //function declaration
void InsertEdge(Graph* G, int a, int b);
void PrintGraph(Graph* G);
void DeleteGraph(Graph* G);
void Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);

int main(int argc, char* argv[]) {          //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char numbers[100], inputs[150];
    fgets(numbers, 100, fin);
    int n = 0;
    countInput(&n, numbers);
    int* nodes = (int*)malloc(sizeof(int) * n);
    parseInput(nodes, numbers, n);

    Graph* G = CreateGraph(nodes, n);
    
    fgets(inputs, 100, fin);
    int len = strlen(inputs), i, j;
    for (i = 0; i < len; i += 4) {
        int a = inputs[i] - '0', b = inputs[i + 2] - '0';
        InsertEdge(G, a, b);
    }

    PrintGraph(G);
    Topsort(G);
    DeleteGraph(G);

    fclose(fin);
    fclose(fout);

    return 0;
}

Graph* CreateGraph(int* nodes, int n) {         //graph create function
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = n;
    graph->node = (int*)malloc(sizeof(int) * graph->size);
    for (int i = 0; i < graph->size; i++) 
        graph->node[i] = nodes[i];
    graph->matrix = (int**)malloc(sizeof(int*) * graph->size);
    for (int i = 0; i < graph->size; i++) {
        graph->matrix[i] = (int*)malloc(sizeof(int) * graph->size);
        for (int j = 0; j < graph->size; j++)
            graph->matrix[i][j] = 0;
    }
    return graph;
}

void InsertEdge(Graph* G, int a, int b) {           //edge insert function
    G->matrix[a][b] = 1;
}

void PrintGraph(Graph* G) {         //graph print function
    fprintf(fout, "   ");
    for (int i = 0; i < G->size; i++)
        fprintf(fout, "%d  ", G->node[i]);
    fprintf(fout, "\n");
    for (int i = 0; i < G->size; i++) {
        fprintf(fout, "%d  ", G->node[i]);
        for (int j = 0; i < G->size; j++)
            fprintf(fout, "%d  ", G->matrix[i][j]);
        fprintf(fout, "\n");
    }
}

void DeleteGraph(Graph* G) {            //graph delete function    
    free(G->node);
    for (int i = 0; i < G->size; i++)
        free(G->matrix[i]);
    free(G->matrix);
    free(G);
}

void Topsort(Graph* G) {            //topological sort function
    Queue* queue = MakeNewQueue(G->size);
}

Queue* MakeNewQueue(int X) {            //queue create function
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->key = (int*)malloc(sizeof(int) * X);
    for (int i = 0; i < X; i++)
        queue->key[i] = 0;
    queue->first = 0;
    queue->rear = 0;
    queue->qsize = 0;
    queue->max_queue_size = X;
    return queue;
}

int IsEmpty(Queue* Q) {         //queue empty check function
    if (Q->qsize == 0)
        return 1;
    return 0;
}

int Dequeue(Queue* Q) {         //dequeue function
    if (IsEmpty(Q)) {
        fprintf(fout, "Queue is empty\n");
        return 0;
    }
    return Q->first++;
}

void Enqueue(Queue* Q, int X) {         //enqueue function
    if (Q->rear == Q->max_queue_size - 1) {
        fprintf(fout, "Queue is full\n");
        return;
    }
    Q->key[++Q->rear] = X;
}

void DeleteQueue(Queue* Q) {            //queue delete function
    if (Q != NULL) {
        if (Q->key != NULL)
            free(Q->key);
        free(Q);
    }
}

void countInput(int* n, char* str) {            //input count function
    int len = strlen(str), i;
    for (i = 0; i < len; i++) {
        if (0 <= str[i] - '0' && str[i] - '0' < 10)
            (*n)++;
    }
}

void parseInput(int* arr, char* str, int n) {           //input parse function
    int len = strlen(str), i;
    int cnt = 0;
    for (i = 0; i < len; i++) {
        if (0 <= str[i] - '0' && str[i] - '0' < 10)
            arr[cnt++] = str[i] - '0';
    }
}
