#include <stdio.h>
#include <stdlib.h>
#include <time.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
    int size_maze;
    int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);       //function declaration
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);

int main(int argc, char* argv[]) {      //main function
    srand((unsigned int)time(NULL));

    int num, i;
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    
    DisjointSets *sets, *maze_print;
    fscanf(fin, "%d", &num);

    sets = (DisjointSets*)malloc(sizeof(DisjointSets));     //memory allocation for sets
    maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));       //memory allocation for maze_print

    init(sets, maze_print, num);
    createMaze(sets, maze_print, num);
    printMaze(maze_print, num);

    freeMaze(sets, maze_print);

    fclose(fin);
    fclose(fout);

    return 0;
}

void init(DisjointSets *sets, DisjointSets *maze_print, int num) {      //maze initialize function
    int i;

    sets->size_maze = num * num;        //set a size of the set
    sets->ptr_arr = (int*)malloc(sizeof(int) * (sets->size_maze + 1));      //memory allocation for ptr_arr
    maze_print->size_maze = num * num * 2;      //set a size of the maze_print
    maze_print->ptr_arr = (int*)malloc(sizeof(int) * (maze_print->size_maze + 1));      //memory allocation for ptr_arr
    for (i = 1; i <= maze_print->size_maze; i++) {
        if (i <= num * num)
            sets->ptr_arr[i] = 0;       //set all elements to 0
        maze_print->ptr_arr[i] = -1;
    }
}

void Union(DisjointSets *sets, int i, int j) {      //maze union function
    int root1 = find(sets, i);
    int root2 = find(sets, j);

    if (root1 == root2)     //if two elements exist in the same set
        return;     //do nothing
    if (sets->ptr_arr[root1] < sets->ptr_arr[root2])        //if root1's rank is bigger than root2's rank
        sets->ptr_arr[root2] = root1;       //union root2 into root1
    else {      //if root1's rank is not bigger than root2's rank
        sets->ptr_arr[root1] = root2;       //union root1 into root2
        if (sets->ptr_arr[root1] == sets->ptr_arr[root2])       //if each rank is equal
            sets->ptr_arr[root2]--;     //increase root2's rank
    }
}

int find(DisjointSets *sets, int i) {       //find function
    if (sets->ptr_arr[i] <= 0)      //if element of the array is 0 or negative number
        return i;       //return the root node as the 'name' of the set
    else        //if element of the array is positive number
        return (sets->ptr_arr[i] = find(sets, sets->ptr_arr[i]));       //follow the pointer to find the root
}

void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num) {        //maze create function
    while (find(sets, 1) != find(sets, num*num)) {
        int rand_wall = rand() % (num * num * 2 - num) + 1;     //create random number
        int cell1, cell2, root1, root2;
        
        if (rand_wall <= num * num) {       //if random number is equal or smaller than set size
            if (rand_wall % num == 0)
                continue;
            cell1 = rand_wall;
            cell2 = rand_wall + 1;
        }
        else {      //if random number is bigger than set size
            cell1 = rand_wall - num * num;      //reduce random number
            cell2 = rand_wall - num * num + num;
        }
        root1 = find(sets, cell1);      //find a root of cell1
        root2 = find(sets, cell2);      //find a root of cell2
        if (root1 == root2)
            continue;
        Union(sets, cell1, cell2);      //union two sets
        maze_print->ptr_arr[rand_wall] = 0;
    }
}

void printMaze(DisjointSets *sets, int num){        //maze print function
    for (int row = 0; row <= num * 2; row++) {
        for (int col = 0; col<=num; col++) {
            if (row % 2 == 0 && col == 0)       //case that even row and column is 0
                fprintf(fout, "+");     //print first +
            else if (col != 0 && (row == 0 || row == num * 2))      //case that  horizontal edge exist
                fprintf(fout, "---+");
            else if (row % 2 != 0 && (col == 0 || col == num)) {
                if (row == 1 && col == 0 || row == num * 2 - 1 && col == num)   //case that vertical edge doesn't exist
                    fprintf(fout, "    ");
                else        //case that vertical edge exist
                    fprintf(fout, "|   ");
            }
            else {      //case that row is not even or column is not 0
                if (row % 2 == 0 && col != 0) {     //case that row is even and column is not 0
                    if (sets->ptr_arr[num * num + num * (row / 2 -1) + col] == 0)
                        fprintf(fout, "   +");
                    else
                        fprintf(fout, "---+");
                }
                else if (row % 2 != 0 && col != 0) {        //case that row is odd and column is not 0
                    if (sets->ptr_arr[(row / 2) * num + col] == 0)
                        fprintf(fout, "    ");
                    else
                        fprintf(fout, "|   ");
                }
            }
        }
        fprintf(fout, "\n");
    }
}

void freeMaze(DisjointSets *set, DisjointSets *maze_print) {        //maze free function
    free(set->ptr_arr);     //free ptr_arr before free set
    free(set);      //free set
    free(maze_print->ptr_arr);      //free ptr_arr before free maze_print
    free(maze_print);       //free maze_print
}
