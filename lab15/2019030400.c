#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl {
    int TableSize;
    List *TheLists;
}HashTbl;

HashTable createTable(int TableSize);           //function declaration
int Iteration(HashTable H, int hashValue, int iteration, int solution);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);

int main(int argc, char *argv[]) {          //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    char solution_str[20];
    int solution, TableSize;

    fscanf(fin, "%s", solution_str);
    if (!strcmp(solution_str, "linear"))
        solution = 1;
    else if (!strcmp(solution_str, "quadratic"))
        solution = 2;
    else {
        fprintf(fout, "error : invalid hashing solution!");
        return 0;
    }

    fscanf(fin, "%d", &TableSize);
    HashTable H = createTable(TableSize);

    char cv;
    int key;
    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':           //insert case
                fscanf(fin, "%d", &key);
                Insert(H, key, solution);
                break;
            case 'f':           //find case
                fscanf(fin, "%d", &key);
                int result = Find(H, key, solution);
                if (result != -1)
                    fprintf(fout, "%d is in the table\n", key);
                else
                    fprintf(fout, "%d is not in the table\n", key);
                break;
            case 'd':           //delete case
                fscanf(fin, "%d", &key);
                Delete(H, key, solution);
                break;
            case 'p':           //print case
                printTable(H);
                break;
        }
    }
    deleteTable(H);

    return 0;
}

HashTable createTable(int TableSize) {          //hash table create function
    HashTable H = (HashTable)malloc(sizeof(HashTbl));           //memory allocation for hash table
    H->TableSize = TableSize;           //set table size
    H->TheLists = (List*)malloc(sizeof(List) * TableSize);          //memory allocation for list
    for (int i = 0; i < TableSize; i++)         //initialize each elements of list to 0
        H->TheLists[i] = 0;
    return H;
}

int Iteration(HashTable H, int hashValue, int iteration, int solution) {            //iteration function
    int hi;
    if (solution == 1)          //if solution is 1, set hash value to linear function
        hi = (hashValue + iteration) % H->TableSize;
    else if (solution == 2)         //if solution is 2, set hash value to quadratic function
        hi = (hashValue + iteration * iteration) % H->TableSize;
    return hi;
}

void Insert(HashTable H, ElementType Key, int solution) {           //key insertion function
    int hValue = Key % H->TableSize, hi, i = 1;
    
    int result = Find(H, Key, solution);            //find the key in hash table
    if (result != -1) {         //if key exists in hash table, print error message and return
        fprintf(fout, "insertion error : %d already exists at address %d\n", Key, result);
        return;
    }

    int count = 0;
    for (int i = 0; i < H->TableSize; i++) {            //iterate from index 0 to last index
        if (H->TheLists[i] != 0)            //if value exists, increase count value
            count++;
    }
    if (count == H->TableSize) {            //if count value is same as table size, this means table is full
        fprintf(fout, "insertion error : table is full\n");         //print error message and return
        return;
    }

    if (H->TheLists[hValue] == 0) {         //start with first hash value, if first hash value index is empty, just insert the key
        H->TheLists[hValue] = Key;
        fprintf(fout, "insert %d into address %d\n", Key, hValue);
    }
    else {          //if first hash value index is not empty
        hi = Iteration(H, hValue, i, solution);         //change hash value to avoid collision
        while (H->TheLists[hi] != 0)        //until find empty space
            hi = Iteration(H, hValue, ++i, solution);
        H->TheLists[hi] = Key;          //if empty space is found, insert the key
        fprintf(fout, "insert %d into address %d\n", Key, hi);
    }
}

void Delete(HashTable H, ElementType Key, int solution) {           //key deletion function
    int result = Find(H, Key, solution);            //find the key using Find function
    if (result == -1) {         //if key doesn't exist in hash table, print error message and return
        fprintf(fout, "deletion error : %d is not in the table\n", Key);
        return;
    }
    H->TheLists[result] = 0;            //if key exists in hash table, change the value to 0(it means value deletion)
    fprintf(fout, "delete %d\n", Key);          //print message
}

int Find(HashTable H, ElementType Key, int solution) {          //key find function
    int hValue = Key % H->TableSize, hi;
    for (int i = 0; i < H->TableSize * 100; i++) {          //iterate using Iteration function up to TableSize * 100 times
        hi = Iteration(H, hValue, i, solution);
        if (H->TheLists[hi] == Key)         //if key exists in hash table, return index of key
            return hi;
    }
    return -1;          //if key doesn't exist in hash table, return -1
}

void printTable(HashTable H) {          //hash table print function
    for (int i = 0; i < H->TableSize; i++)          //iterate from index 0 to last index
        fprintf(fout, "%d ", H->TheLists[i]);           //print each elements
    fprintf(fout, "\n");
}

void deleteTable(HashTable H) {         //hash table deletion function
    free(H->TheLists);
    free(H);
}
