#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST {
    int value;
    struct BST* left;
    struct BST* right;
} BST;

Tree insertNode(Tree root, int key);        //function declaration
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]) {         //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    char cv;
    int key;

    Tree root = NULL;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                root = insertNode(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (findNode(root, key))            //if key is in the tree, print the value
                    fprintf(fout, "%d is in the tree\n", key);
                else            //if key is not in the tree, print error message
                    fprintf(fout, "finding error: %d is not in the tree\n", key);
                break;
            case 'd':
                fscanf(fin, "%d", &key);
                if (findNode(root, key)) {          //if key is in the tree, delete that node and print message
                    root = deleteNode(root, key);
                    fprintf(fout, "delete %d\n", key);
                }
                else {          //if key is not in the tree, print error message
                    fprintf(fout, "deletion error: %d is not in the tree\n", key);
                }
                break;
            case 'p':
                fscanf(fin, "%c", &cv);
                if (cv == 'i') {
                    if (root == NULL)           //if tree is empty, print message
                        fprintf(fout, "tree is empty");
                    else            //if tree is not empty, print tree inorder
                        printInorder(root);
                }
                fprintf(fout, "\n");
                break;
        }
    }
    deleteTree(root);
}

Tree insertNode(Tree root, int key) {           //node insertion function
    if (root == NULL) {         //if root is null, allocate memory and insert value
        root = (Tree)malloc(sizeof(BST));
        if (root == NULL) {         //if memory allocation fails, print error message
            fprintf(fout, "out of space!\n");
        }
        root->value = key;
        root->left = NULL;
        root->right = NULL;
        fprintf(fout, "insert %d\n", key);
    }
    else if (key == root->value) {      //if key is exist in tree, print error message
        fprintf(fout, "insertion error: %d is already in the tree\n", key);
    }
    else if (key < root->value)         //if key is smaller than root's value, go to left subtree recursively
        root->left = insertNode(root->left, key);
    else if (key > root->value)         //if key is bigger than root's value, go to right subtree recursively
        root->right = insertNode(root->right, key);
    return root;
}

Tree deleteNode(Tree root, int key) {           //node deletion function
    Tree tmp;
    if (key < root->value)      //if key is smaller than root's value, delete node in left subtree recursively
        root->left = deleteNode(root->left, key);
    else if (key > root->value)         //if key is bigger than root's value, delete node in right subtree recursively
        root->right = deleteNode(root->right, key);
    else {      //if we find the node to delete
        if (root->left != NULL && root->right != NULL) {        //if node has two children
            tmp = root->right;      //go to right subtree
            while (tmp->left != NULL)       //find smallest value in right subtree
                tmp = tmp->left;
            root->value = tmp->value;       //move smallest value of left subtree to node
            root->right = deleteNode(root->right, root->value);     //delete the node with smallest value in right subtree
        }
        else {      //if node has one child or no child
            tmp = root;
            if (root->left == NULL)         //if node has child in right subtree, connect right child to parent
                root = root->right;
            else if (root->right == NULL)           //if node has child in left subtree, connect left child to parent
                root = root->left; 
            free(tmp);      //delete node
        }
    }
    return root;
}

int findNode(Tree root, int key) {          //node search function
    if (root == NULL)       //if key is not in the tree, return 0
        return 0;
    else if (key < root->value)     //if key is smaller than root's value, find in left subtree recursively
        return findNode(root->left, key);
    else if (key > root->value)     //if key is bigger than root's value, find in right subtree recursively
        return findNode(root->right, key);
    else            //if key is in the tree, return 1
        return 1;
}

void printInorder(Tree root) {          //function that print the tree inorder
    if (root == NULL)
        return;
    printInorder(root->left);           //print left subtree(recursive)
    if (root->left != NULL)
        fprintf(fout, " ");
    fprintf(fout, "%d", root->value);           //print value
    if (root->right != NULL)
        fprintf(fout, " ");
    printInorder(root->right);          //print right subtree(recursive)
}

void deleteTree(Tree root) {            //tree deletion function
    if (root->left != NULL)         //if tree has left subtree, delete left subtree
        deleteTree(root->left);
    else if (root->left == NULL && root->right != NULL)         //if tree has right subtree, delete right subtree
        deleteTree(root->right);
    else if (root->left == NULL && root->right == NULL)         //if node has no child, delete node
        free(root);
}
