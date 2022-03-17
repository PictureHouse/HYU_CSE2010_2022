#include <stdio.h>
#include <stdlib.h>

FILE *input; 
FILE *output;

typedef struct Node *PtrToNode; //Structure declaration
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node
{
    ElementType element;
    Position next;
};

List MakeEmpty(List L); //function declaration
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
void PrintList(List L);
void Delete(ElementType X, List L);
Position Find(ElementType X, List);
Position FindPrevious (ElementType X, List L);
void DeleteList (List L);

int main(int argc, char *argv[]) {  //main function
	char command;
	int key1, key2;
	FILE *input;
	Position header = NULL;
	Position tmp = NULL; 
	if (argc == 1) {    //file open
		input = fopen("input.txt", "r"); 
		output = fopen("output.txt", "w");
    }
	else {
		input = fopen(argv[1], "r"); 
		output = fopen(argv[2], "w");
    }
	header = MakeEmpty(header); //make header empty
	while (1) {
		command = fgetc(input);     //get char from input
		if (feof(input)) break; 
		switch (command) { 
			case 'i':   //'i'nsert case
				fscanf(input, "%d %d", &key1, &key2); 
				//duplication check
                if (key2 == -1) {   //if key2 is -1, insert the value next to header
                    Insert(key1, header, header);   //insert
                    break;
                }
				tmp = Find(key1, header);
				if (tmp != NULL) {  //print the error message
					fprintf(output, "insertion(%d) failed : the key already exists in the list\n", key1);
					break; 
				}
                tmp = Find(key2, header); 
				if (tmp == NULL) {  //print the error message
			        fprintf(output, "insertion(%d) failed : can not find the location to be inserted\n", key1);
					break; 
                }
                
				Insert(key1, header, tmp); //insert
				break;
			case 'd':   //'d'elete case
				fscanf(input, "%d", &key1);
				Delete(key1, header);
				break; 
			case 'f':   //'f'ind case
				fscanf(input, "%d", &key1);
				tmp = FindPrevious(key1, header); 
				if (IsLast(tmp, header))    //print the error message
					fprintf(output, "finding(%d) failed : element %d is not in the list\n", key1, key1); 
				else {  //printf the key value of the previous node
					if (tmp->element > 0)
						fprintf(output, "key of the previous node of %d is %d\n", key1, tmp->element);
					else
						fprintf(output, "key of the previous node of %d is head\n", key1);
				}
				break; 
			case 'p':   //'p'rint case
				PrintList(header);
				break;
			default:    //another case
				break;
			}
	}

	DeleteList(header); //delete the list before end
	fclose(input);  //close file
	fclose(output); 
	return 0;
}

List MakeEmpty(List L) {    //make the list empty
    L = (List)malloc(sizeof(struct Node));  //memory allocation
    L->element = -1;
    L->next = NULL;
    return L;
}

int IsEmpty(List L) {
    return L->next == NULL; //if the list is empty, return true
}

int IsLast(Position P, List L) {
    return P->next == NULL; //if this is the last node, return true
}

void Insert(ElementType X, List L, Position P) {    //insert new node
    Position tmp;   //make a new node
    tmp = (Position)malloc(sizeof(struct Node));
    tmp->element = X;   //give X to node
    tmp->next = P->next;    //remake connections
    P->next = tmp;
}

void PrintList(List L) {    //print the list
    Position P;
    P = L->next;
    if (P == NULL) {    //if the list is empty
        fprintf(output, "empty list!\n");
    }
    else {  //if the list is not empty
        while (P != NULL) {
            fprintf(output, "key:%d ", P->element);  //print elements of list
            P = P->next;    //step by step
        }
    }
}

void Delete(ElementType X, List L) {    //delete the node
    Position P, tmp;
    P = FindPrevious(X, L);
    if (IsLast(P, L))   //there is no element that we want to delete
        fprintf(output, "deletion(%d) failed : element %d is not in the list\n", X, X);
    else {  //the element that we want to delete is exist in the list
        tmp = P->next;
        P->next = tmp->next;    //remake the connection
        free(tmp);  //i made a mistake in this line and i fixed it
    }
}

Position Find(ElementType X, List L) {  //find the element
    Position P;    
    P = L->next;
    while (P != NULL && P->element != X) {  //if position is not the end and the element is not X, go to next node.
        P = P->next;
    }
    return P;
}

Position FindPrevious(ElementType X, List L) {  //find the element in different way
    Position P;
    P = L;
    while (P->next != NULL && P->next->element != X) {  //if the next is not the end and the next element is not X, go to next node
        P = P->next;
    }
    return P;
}

void DeleteList(List L) {   //delete the list and return memory
    Position P, tmp;
    P = L->next;
    L->next = NULL;
    while (P != NULL) {
        tmp = P->next;
        free(P);    //memory return
        P = tmp;
    }
}
