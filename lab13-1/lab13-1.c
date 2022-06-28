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
    Graph* graph = (Graph*)malloc(sizeof(Graph));           //memory allocation for graph
    graph->size = n;
    graph->node = (int*)malloc(sizeof(int) * graph->size);          //memory allocation for node array
    for (int i = 0; i < graph->size; i++) {         //bubble sorting nodes
        int tmp;
        for (int j = 0; j < graph->size - 1; j++) {
            if (nodes[j] > nodes[j + 1]) {   
                tmp = nodes[j];
                nodes[j] = nodes[j + 1];
                nodes[j + 1] = tmp;
            }
        }
    }
    for (int i = 0; i < graph->size; i++)           //input nodes to node array
        graph->node[i] = nodes[i];
    graph->matrix = (int**)malloc(sizeof(int*) * graph->size);          //memory allocation for 2-dimentional array(matrix) and initialize with 0
    for (int i = 0; i < graph->size; i++) {
        graph->matrix[i] = (int*)malloc(sizeof(int) * graph->size);
        for (int j = 0; j < graph->size; j++)
            graph->matrix[i][j] = 0;
    }
    return graph;
}

void InsertEdge(Graph* G, int a, int b) {           //edge insert function
    for (int i = 0; i < G->size; i++) {         //if edge exist between nodes, insert 1 to the space that indicates the edge
        if (G->node[i] == a) {
            for (int j = 0; j < G->size; j++) {
                if (G->node[j] == b) {
                    G->matrix[i][j] = 1;
                    return;
                }
            }
        }
    }
}

void PrintGraph(Graph* G) {         //graph print function
    fprintf(fout, "   ");           //print node key value
    for (int i = 0; i < G->size; i++)
        fprintf(fout, "%d  ", G->node[i]);
    fprintf(fout, "\n");
    for (int i = 0; i < G->size; i++) {         //print matrix
        fprintf(fout, "%d  ", G->node[i]);
        for (int j = 0; j < G->size; j++)
            fprintf(fout, "%d  ", G->matrix[i][j]);
        fprintf(fout, "\n");
    }
    fprintf(fout, "\n");
}

void DeleteGraph(Graph* G) {            //graph delete function    
    free(G->node);          //free node and matrix and then graph
    for (int i = 0; i < G->size; i++)
        free(G->matrix[i]);
    free(G->matrix);
    free(G);
}

void Topsort(Graph* G) {            //topological sort function
    Queue* queue = MakeNewQueue(G->size);
    int v, cnt = 0, col = 0;
    int in_degrees[G->size];        //indegree array that show the indegrees of nodes
    
    while (col < G->size) {         //initialize indegree array
        for (int i = 0; i < G->size; i++) {
            if (G->matrix[i][col] == 1)
                cnt++;
        }
        in_degrees[col++] = cnt;
        cnt = 0;
    }
    for (int i = 0; i < G->size; i++) {         //input nodes that have no indegree to queue
        if (in_degrees[i] == 0) {
            Enqueue(queue, G->node[i]);
        }
    }
    if (IsEmpty(queue)) {           //if cycle exist, print error message
        fprintf(fout, "sorting error : cycle!\n");
        return;
    }
    while (!IsEmpty(queue)) {           //while queue is not empty, repeat topological sorting using enqueue and dequeue
        v = Dequeue(queue);         //dequeue the first value of queue and print it
        fprintf(fout, "%d ", v);
        for (int i = 0; i < G->size; i++) {
            for (int j = 0; j < G->size; j++) {
                if (G->node[i] == v) {
                    if (G->matrix[i][j] == 1) {
                        G->matrix[i][j]--;          //after dequeue, decrease the indegree of node that the dequeued node point
                        if (--in_degrees[j] == 0) {     //if this node becomes no indegreed, enqueue this node to queue
                            Enqueue(queue, G->node[j]);
                        }
                    }
                }
            }
        }
    }
    fprintf(fout, "\n");
    DeleteQueue(queue);         //delete queue after sorting
}

Queue* MakeNewQueue(int X) {            //queue create function
    Queue* queue = (Queue*)malloc(sizeof(Queue));           //memory allocation for queue
    queue->key = (int*)malloc(sizeof(int) * X);         //memory allocation for key array
    for (int i = 0; i < X; i++)         //initialize key array
        queue->key[i] = 0;
    queue->first = 0;           //initialize values of the queue
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
    int value;
    if (Q->qsize == 0) {            //if queue is empty, print error message
		fprintf(fout, "Queue is empty\n");
		return 0;
	}
	value = Q->key[Q->first];       //pop the first value of queue
    Q->qsize--;     //decrease queue size
	Q->first = (Q->first + 1) % Q->max_queue_size;      //reset first value indicator
	return value;
}

void Enqueue(Queue* Q, int X) {         //enqueue function
    if (Q->qsize == Q->max_queue_size) {        //if queue is full, print error message
		fprintf(fout, "Queue is full\n");
		return;
	}
	Q->key[Q->rear] = X;        //input new value
    Q->qsize++;     //increase queue size
	Q->rear = (Q->rear + 1) % Q->max_queue_size;        //reset rear value indicator
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
