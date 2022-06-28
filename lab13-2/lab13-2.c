#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
    int vertex;
    int dist;
    int prev;
}Node;

typedef struct Graph {
    int size;
    int** vertices;
    Node* nodes;
}Graph;

typedef struct Heap {
    int Capacity;
    int Size;
    Node* Element;
}Heap;

Graph* createGraph(int size);           //function declaration
void deleteGraph(Graph* g);
void printShortestPath(Graph* g);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);

void main(int argc, char *argv[]) {         //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Graph* g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = createGraph(size + 1);

    char tmp = 0;
    while (tmp != '\n' && tmp != EOF) {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->vertices[node1][node2] = weight;
        tmp = fgetc(fin);
    }

    printShortestPath(g);

    deleteGraph(g);
}

Graph* createGraph(int size) {          //graph create function
    Graph* graph = (Graph*)malloc(sizeof(Graph));           //memory allocation for graph
    graph->size = size;         //set graph size
    graph->vertices = (int**)malloc(sizeof(int*) * size);           //memory allocation for vertices matrix
    for (int i = 0; i < size; i++)
        graph->vertices[i] = (int*)malloc(sizeof(int) * size);
    for (int i = 1; i < size; i++) {            //initialize vertices matrix
        for (int j = 1; j < size; j++)
            graph->vertices[i][j] = INF;
    }
    graph->nodes = (Node*)malloc(sizeof(Node) * size);          //memory allocation for nodes array
    for (int i = 1; i < size; i++) {            //initialize nodes array
        graph->nodes[i].vertex = i;
        graph->nodes[i].dist = INF;
        graph->nodes[i].prev = -1;
    }
    return graph;
}

void deleteGraph(Graph* g) {            //graph delete function
    free(g->nodes);         //free nodes array
    for (int i = 0; i < g->size; i++)           //free vertices matrix
        free(g->vertices[i]);
    free(g->vertices);
    free(g);            //free graph
}

void printShortestPath(Graph* g) {          //shortest path print function
    Heap* heap = createMinHeap(g->size);            //creat min heap
	Node node;
	int* found = (int*)malloc(sizeof(int) * g->size);           //memory allocation for found array
	for (int i = 0; i < g->size; i++)           //initialize found array
		found[i] = 0;
	found[1] = 1;           //set index 1 value of found array to 1
	for (int i = 2; i < g->size; i++) {         //traverse from 2 node to last node
		if (g->vertices[1][i] != INF) {         //if node status is not infinity
			g->nodes[i].dist = g->vertices[1][i];           //set distance of i node
			g->nodes[i].prev = 1;           //set previous node of i node to 1 node
			insertToMinHeap(heap, g->nodes[i].vertex, g->nodes[i].dist);            //insert node to min heap
		}
	}
	while(heap->Size != 0){
		node = deleteMin(heap);         //delete min value node from heap and set node to that value
		found[node.vertex] = 1;
		for (int i = 2; i < g->size; i++){          //traverse from 2 node to last node
			if (node.dist + g->vertices[node.vertex][i] < g->nodes[i].dist && !found[i]){           //if found shorter distance path
				g->nodes[i].dist = node.dist + g->vertices[node.vertex][i];         //set distance
				g->nodes[i].prev = node.vertex;         //set min value node to previous node
				insertToMinHeap(heap, g->nodes[i].vertex, g->nodes[i].dist);            //insert node to min heap
			}
		}
	}
	for (int i = 2; i < g->size; i++){          //traverse from 2 node to last node
		int curNode = i, j = 0;
		int* path = (int*)malloc(sizeof(int) * g->size);            //memory allocation for path array
		while (g->nodes[curNode].prev != -1){       //while the node can be reached
			path[j++] = g->nodes[curNode].vertex;           //increase j value and set j'th value of path array to current node
			curNode = g->nodes[curNode].prev;           //set current node to current node's previous node
		}
		if (j == 0)         //if cannot reach to the node, print message
			fprintf(fout, "can not reach to node %d\n", curNode);
		else {
			path[j] = 1;
			for (int k = j; k > 0; k--)         //print path from 1 to target node
				fprintf(fout, "%d->", path[k]);	
			fprintf(fout, "%d ", path[0]);
			fprintf(fout, "(cost : %d)\n", g->nodes[i].dist);           //print total cost of path
		}
	}
}

Heap* createMinHeap(int heapSize) {         //min heap create function
    Heap* heap = (Heap*)malloc(sizeof(Heap));           //memory allocation for min heap
    heap->Capacity = heapSize;          //initialize min heap
    heap->Size = 0;
    heap->Element = (Node*)malloc(sizeof(Node));        //memory allocation for element array
    return heap;
}

void deleteMinHeap(Heap* minHeap) {         //min heap delete function
    free(minHeap->Element);         //free element array
    free(minHeap);          //free min heap
}

void insertToMinHeap(Heap* minHeap, int vertex, int distance) {         //insert to min heap function
    Node tmp;
    minHeap->Size++;            //increase heap size
    minHeap->Element[minHeap->Size].vertex = vertex;            //input new value
    minHeap->Element[minHeap->Size].dist = distance;
    for (int i = minHeap->Size; minHeap->Element[i / 2].dist > minHeap->Element[i].dist && i / 2 > 0; i /= 2) {         //rearrange the min heap by swap
        tmp = minHeap->Element[i];
        minHeap->Element[i] = minHeap->Element[i / 2];
        minHeap->Element[i / 2] = tmp;
    }
}

Node deleteMin(Heap* minHeap) {         //min element delete function
    Node min_element = minHeap->Element[1];         //pop min element from min heap
    Node last_element = minHeap->Element[minHeap->Size--];          //decrease heap size
    int i, child;
    for (i = 1; 2 * i <= minHeap->Size; i = child) {            //after deletion, rearrange the heap
        if (minHeap->Element[2 * i].dist < minHeap->Element[2 * i + 1].dist)            //if left child distance is shorter than right child distance, set left child is child
            child = 2 * i;           
        else            //if right child distance is shorter than left child distance, set right child is child
            child = 2 * i + 1;
        if (minHeap->Element[child].dist < last_element.dist)           //if child distance is shorter than last element distance, pull up the child
            minHeap->Element[i] = minHeap->Element[child];
        else            //if last element distance is shorter than child distance, break the for loop
            break;
    }
    minHeap->Element[i] = last_element;         //set i'th element to last element value
    return min_element;             //return original min element
}
