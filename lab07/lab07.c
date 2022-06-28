#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode {
    ElementType element;
    AVLTree left, right;
    int height;
} AVLNode;

int Height(AVLTree T);          //function declaration
int HeightDiff(int H1, int H2);
AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
Position SingleRotateWithLeft(Position node);
Position SingleRotateWithRight(Position node);
Position DoubleRotateWithLeft(Position node);
Position DoubleRotateWithRight(Position node);
void PrintInorder(AVLTree T);
void DeleteTree(AVLTree T);

int main(int argc, char *argv[]) {          //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    AVLTree Tree = NULL;
    char cv;
    int key;

    while(!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);       //implement insertion
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);       //implement deletion
                break;
        }
        PrintInorder(Tree);         //print tree in inorder traversal
        fprintf(fout, "\n");
    }

    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);

    return 0;
}

int Height(AVLTree T) {         //height return function
    if (T == NULL)
        return -1;
    return T->height;
}

int HeightDiff(int H1, int H2) {        //height difference return function
    return H1 - H2;
}

AVLTree Insert(ElementType X, AVLTree T) {      //node insertion function
    if (T == NULL) {            //if T is null, allocate memory and insert value
        T = (AVLTree)malloc(sizeof(AVLNode));
        if (T == NULL) {            //if memory allocation fails, print error message
            fprintf(fout, "out of space!\n");
            return 0;
        }
        T->element = X;
        T->left = NULL;
        T->right = NULL;
        T->height = 0;
    }
    else {
        if (X < T->element) {
            T->left = Insert(X, T->left);
            if (HeightDiff(Height(T->left), Height(T->right)) >= 2) {
                if (X < T->left->element)           //left subtree of the left child case
                    T = SingleRotateWithLeft(T);
                else            //right subtree of the left child case
                    T = DoubleRotateWithLeft(T);
            }
        }
        else if (X > T->element) {
            T->right = Insert(X, T->right);
            if (HeightDiff(Height(T->left), Height(T->right)) <= -2) {
                if (X > T->right->element)          //right subtree of the right child case
                    T = SingleRotateWithRight(T);
                else            //left subtree of the right child case
                    T = DoubleRotateWithRight(T);
            }
        }
        else {          //if X is exist in the tree, print error message
            fprintf(fout, "insertion error : %d is already in the tree!\n", X);
        }
    }
    if (Height(T->left) >= Height(T->right))            //update height value
        T->height = Height(T->left) + 1;
    else
        T->height = Height(T->right) + 1;
    return T;
}

AVLTree Delete(ElementType X, AVLTree T) {          //node deletion function
    AVLTree tmp;
    if (T == NULL) {            //if X is not in tree, print error message
        fprintf(fout, "deletion error : %d is not in the tree!\n", X);
        return NULL;
    }
    else if (X < T->element)
        T->left = Delete(X, T->left);
    else if (X > T->element)
        T->right = Delete(X, T->right);
    else {
        if (T->left != NULL && T->right != NULL) {
            tmp = T->right;
            while (tmp->left != NULL)
                tmp = tmp->left;
            T->element = tmp->element;
            T->right = Delete(T->element, T->right);
        }
        else {
            tmp = T;
            if (T->left == NULL)
                T = T->right;
            else if (T->right == NULL)
                T = T->left; 
            free(tmp);
        }
    }
   
    if (T == NULL)
        return T;

    if (Height(T->left) >= Height(T->right))            //update height value
        T->height = Height(T->left) + 1;
    else
        T->height = Height(T->right) + 1;

    int balance = HeightDiff(Height(T->left), Height(T->right));            //if tree is unbalaced, rotate tree
    if (balance >= 2 && HeightDiff(Height(T->left->left), Height(T->left->right)) >= 0)         //left subtree of the left child case
        T = SingleRotateWithLeft(T);
    if (balance >= 2 && HeightDiff(Height(T->left->left), Height(T->left->right)) < 0)          //right subtree of the left child case
        T = DoubleRotateWithLeft(T);
    if (balance <= -2 && HeightDiff(Height(T->right->left), Height(T->right->right)) <= 0)      //right subtree of the right child case
        T = SingleRotateWithRight(T);
    if (balance <= -2 && HeightDiff(Height(T->right->left), Height(T->right->right)) > 0)       //left subtree of the right child case
        T = DoubleRotateWithRight(T);

    return T;
}

Position SingleRotateWithLeft(Position node) {          //single rotate function(clockwise)
    Position new;
    new = node->left;
    node->left = new->right;
    new->right = node;
    if (Height(node->left) >= Height(node->right))          //update height value
        node->height = Height(node->left) + 1;
    else
        node->height = Height(node->right) + 1;
    if (Height(new->left) >= node->height)
        new->height = Height(new->left) + 1;
    else
        new->height = node->height + 1;
    return new;
}

Position SingleRotateWithRight(Position node) {         //single rotate function(counterclockwise)
    Position new;
    new = node->right;
    node->right = new->left;
    new->left = node;
    if (Height(node->left) >= Height(node->right))          //update height value
        node->height = Height(node->left) + 1;
    else
        node->height = Height(node->right) + 1;
    if (node->height >= Height(new->right))
        new->height = node->height + 1;
    else
        new->height = Height(new->right) + 1;
    return new;
}

Position DoubleRotateWithLeft(Position node) {          //double rotate function(counterclockwise->clockwise)
    Position new;
    node->left = SingleRotateWithRight(node->left);         //implement counterclockwise single rotate function
    new = SingleRotateWithLeft(node);           //implement clockwise single rotate function
    return new;
}

Position DoubleRotateWithRight(Position node) {         //double rotate function(clockwise->counterclockwise)
    Position new;
    node->right = SingleRotateWithLeft(node->right);        //implement clockwise single rotate function
    new = SingleRotateWithRight(node);          //implement counterclockwise single rotate function
    return new;
}

void PrintInorder(AVLTree T) {          //function that print the tree inorder
    if (T == NULL)
        return;
    PrintInorder(T->left);
    if (T->left != NULL)
        fprintf(fout, " ");
    fprintf(fout, "%d(%d)", T->element, T->height);
    if (T->right != NULL)
        fprintf(fout, " ");
    PrintInorder(T->right);
}
    
void DeleteTree(AVLTree T) {            //tree deletion function
    if (T->left != NULL)
        DeleteTree(T->left);
    if (T->left == NULL && T->right != NULL)
        DeleteTree(T->right);
    if (T->left == NULL && T->right == NULL)
        free(T);
}
