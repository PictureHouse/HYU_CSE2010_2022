#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
    int left_thread;                                //flag if ptr is thread
    ThreadedPtr left_child;
    ElementType data;
    ThreadedPtr right_child;
    int right_thread;                               //flag if ptr is thread
} ThreadedTree;

ThreadedPtr CreateTree();                           //function declaration
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx);
void printInorder(ThreadedPtr root);
void DeleteTree(ThreadedPtr root);

int main(int argc, char *argv[]) {                  //main function
     fin = fopen(argv[1], "r");
     fout = fopen(argv[2], "w");
 
     ThreadedPtr root = CreateTree();
 
     int NUM_NODES;
     fscanf(fin, "%d", &NUM_NODES);
 
     int root_idx = 0, idx = 0;
 
     while (++idx <= NUM_NODES) {
         ElementType data;
         fscanf(fin, "%d", &data);
         if (Insert(root, root_idx, data, idx) == 0) {      //do insertion
             return 0;
         }
     }
 
     printInorder(root);                            //print tree
     DeleteTree(root);                              //delete tree

     fclose(fin);
     fclose(fout);
 
     return 0;
 }

ThreadedPtr CreateTree() {                          //tree create function
    ThreadedPtr tree = NULL;
    tree = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if (tree == NULL) {                             //if dynamic allocation is failed
        fprintf(fout, "Out of Space!\n");           //print error message
        return NULL;
    }
    tree->data = -1;                                //initialize the tree
    tree->left_thread = 1;
    tree->left_child = tree;
    tree->right_thread = 1;
    tree->right_child = tree;
    return tree;
}

int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx) {     //node insert function
    ThreadedPtr node = (ThreadedPtr)malloc(sizeof(ThreadedTree));
    if(node == NULL) {                              //if dynamic allocation is failed
        fprintf(fout, "insertion failed!\n");       //print error message
        return 0;
    }
    node->data = data;                              //initialize the node
    node->left_thread = 1;
    node->right_thread = 1;

    if (idx == 1) {                                 //if this is first node
        root->right_thread = 0;                     //connect this node to root's right_child
        root->right_child = node;
        node->left_child = root;
        node->right_child = root;
        return 1;
    }

    ThreadedPtr parent = root->right_child;
    int parent_idx = 1;

    while (parent_idx != idx / 2) {                 //find the place to insert using index
        int temp_idx = idx;                         //make temporary index
        while (parent_idx != temp_idx / 2) {
            temp_idx /= 2;                          //reduce temp_idx value
        }
        if (temp_idx == parent_idx * 2) {           //if temp_idx == parent_idx * 2
            parent_idx = parent_idx * 2;            //increase parent_idx to parent_idx * 2
            parent = parent->left_child;            //go to left
        }
        else {                                      //if temp_idx == parent_idx * 2 + 1
           parent_idx = parent_idx * 2 + 1;         //increase parent_idx to parent_idx * 2 + 1
           parent = parent->right_child;            //go to right
        }
    }

    if (idx == parent_idx * 2) {                    //if idx == parent_idx * 2
        node->left_child = parent->left_child;      //connect this node to parent's left_child
        node->right_child = parent;
        parent->left_thread = 0;
        parent->left_child = node;
    }
    else {                                          //if idx == parent_idx * 2 + 1
        node->left_child = parent;                  //connect this node to parent's right_child
        node->right_child = parent->right_child;
        parent->right_thread = 0;
        parent->right_child = node;
    }
    return 1;
}

void printInorder(ThreadedPtr root) {               //print function(print the tree by inorder traversal)
    ThreadedPtr current = root->right_child;
    while (current->left_thread == 0) {             //while current node's left_child doesn't contain thread 
        current = current->left_child;
    }  
    fprintf(fout, "inorder traversal :");

    while (current != root) {                       //while current node is not root
        fprintf(fout, " %d", current->data);

        if (current->right_thread == 1) {           //if current node's right_child contains thread
            current = current->right_child;         
        }
        else {                                      //if current node's right_child doesn't contain thread
            current = current->right_child;         
            while (current->left_thread == 0) {     //and while current node's left_child doesn't contain thread
                current = current->left_child;      
            }
        }
    }
}

void DeleteTree(ThreadedPtr root) {                 //delete function(delete nodes by inorder traversal)
    ThreadedPtr current = root->right_child;
    ThreadedPtr tmp;
    while (current->left_thread == 0) {             //while current node's left_child doesn't contain thread
        current = current->left_child;
    }
    while (current != root) {                       //while current not is not root
        if (current->right_thread == 1) {           //if current node's right_child contains thread
            tmp = current;                          //left flag
            current = current->right_child;         //and go to next
            free(tmp);                              //free tmp node
        }
        else {                                      //if current node's right_child doesn't contain thread
            tmp = current;                          //left flag
            current = current->right_child;         //and go to next
            free(tmp);                              //free tmp node
            while (current->left_thread == 0) {     //and while current node's left_child doesn't contain thread
                current = current->left_child;
            }
        }
    }
    free(root);                                     //free root
}
