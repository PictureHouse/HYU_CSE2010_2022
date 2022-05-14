#include <stdio.h>
#include <stdlib.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct {
    int Capacity;
    int Size;
    int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);     //function declaration
void Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
void PrintHeap(Heap* heap);
int IsFull(Heap *heap);

int main(int argc, char* argv[]) {      //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char cv;
    Heap* maxHeap;
    int heapSize, key, max_element;

    while(!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'n':
                fscanf(fin, "%d", &heapSize);
                maxHeap = CreateHeap(heapSize);
                break;
            case 'i':
                fscanf(fin, "%d", &key);
                Insert(maxHeap, key);
                break;
            case 'd':
                max_element = DeleteMax(maxHeap);
                if(max_element != -INF) {
                    fprintf(fout, "max element : %d deleted\n", max_element);
                }
                break;
            case 'p':
                PrintHeap(maxHeap);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (Find(maxHeap, key))
                    fprintf(fout, "%d is in the heap\n", key);
                else
                    fprintf(fout, "finding error : %d is not in the heap\n", key);
                break;
        }
    }

    return 0;
}

Heap* CreateHeap(int heapSize) {        //heap create function
    Heap* heap = (Heap*)malloc(sizeof(Heap));       //memory allocation for heap
    heap->Capacity = heapSize;      //set heap capacity and size
    heap->Size = 0;
    heap->Elements = (int*)malloc(sizeof(int) * (heapSize + 1));        //memory allocation for array
    for (int i = 0; i < heapSize + 1; i++)      //initialize array elements to zero
        heap->Elements[i] = 0;
    return heap;
}

void Insert(Heap *heap, int value) {        //insert function
    int tmp;
    if (Find(heap, value)) {        //if the value is exist in the heap, print error message
        fprintf(fout, "insert error : %d is already in the heap\n", value);
        return;
    }
    if (IsFull(heap)) {         //if heap is empty, print error message
        fprintf(fout, "insert error : heap is full\n");
        return;   
    }
    heap->Elements[++heap->Size] = value;       //increase heap size and input the value
    for (int i = heap->Size; heap->Elements[i / 2] < heap->Elements[i] && i / 2 > 0; i /= 2) {      //rearrange the max heap by swap
        tmp = heap->Elements[i];
        heap->Elements[i] = heap->Elements[i / 2];
        heap->Elements[i / 2] = tmp;
    }
    fprintf(fout, "insert %d\n", value);
}

int Find(Heap *heap, int value) {       //find value function
    for (int i = 1; i <= heap->Size; i++) {     //find value from index 1 to index that is heap size
        if (heap->Elements[i] == value)     //if the value exist, return 1
            return 1;
    }
    return 0;       //if the value doesn't exist, return 0
}

int DeleteMax(Heap* heap) {     //max element delete function
    if (heap->Size == 0) {      //if heap is empty, print error message
        fprintf(fout, "delete error : heap is empty\n");
        return -INF;
    }
    else {      //if heap is not empty, delete the max element
        int max_element = heap->Elements[1];
        int last_element = heap->Elements[heap->Size--];
        int i, child;
        for (i = 1; 2 * i <= heap->Size; i = child) {       //after deletion, rearrange the heap
            if (heap->Elements[2 * i] > heap->Elements[2 * i + 1])      //if left child is bigger than right child, set left child is child
                child = 2 * i;            
            else        //if right child is bigger than left child, set right child is child
                child = 2 * i + 1;
            if (heap->Elements[child] > last_element)       //if child is bigger than last element, pull up the child
                heap->Elements[i] = heap->Elements[child];
            else        //if last element is bigger than child, break the for loop
                break;
        }
        heap->Elements[i] = last_element;       //set i'th element to last element value
        return max_element;     //return original max element
    }
}

void PrintHeap(Heap* heap) {        //heap print function
    if (heap->Size == 0)        //if heap is empty, print error message
        fprintf(fout, "print error : heap is empty\n");
    else {      //if heap is not empty, print elements in level order
        for (int i = 1; i <= heap->Size; i++) 
            fprintf(fout, "%d ", heap->Elements[i]);
        fprintf(fout, "\n");
    }
}

int IsFull(Heap *heap) {        //heap full check function
    if (heap->Size >= heap->Capacity)       //if heap is full, return 1
        return 1;
    else        //if heap is not full, ruturn 0
        return 0;
}
