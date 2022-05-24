#include <stdio.h>
#include <stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode {
    int order;
    int size;           //number of keys
    BNodePtr *child;
    int *key;
    int is_leaf;
}BNode;

BNodePtr CreateTree(int order);             //function declaration
void Insert(BNodePtr root, int key);
int Find(BNodePtr root, int key);
void PrintTree(BNodePtr root);
void DeleteTree(BNodePtr root);

int main(int argc, char* argv[]) {          //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    int order;
    fscanf(fin, "%d", &order);
    BNodePtr root = CreateTree(order);

    char cv;
    int key;
    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch(cv) {
            case 'i':
                fscanf(fin, "%d", &key);
                if (Find(root, key))
                    fprintf(fout, "insert error : key %d is already in the tree!\n", key);
                else
                    Insert(root, key);
                break;
            case 'f':
                fscanf(fin, "%d", &key);
                if (Find(root, key))
                    fprintf(fout, "key %d found\n", key);
                else
                    fprintf(fout, "finding error : key %d is not in the tree!\n", key);
                break;
            case 'p':
                if (root->size == 0)
                    fprintf(fout, "print error : tree is empty!");
                else
                   PrintTree(root);
                fprintf(fout, "\n");
                break;
        }
    }

    DeleteTree(root);
    fclose(fin);
    fclose(fout);

    return 0;
}

BNodePtr CreateTree(int order) {            //node creat function
    BNodePtr tree;
    tree = (BNodePtr)malloc(sizeof(BNode));         //memory allocation of the node
    tree->order = order;        //set the order of the node
    tree->size = 0;     //initialize the number of keys
    tree->child = (BNodePtr*)malloc(sizeof(BNodePtr) * order);          //memory allocation of the child list
    for (int i = 0; i < order; i++)         //initialize child list to null
        tree->child[i] = NULL;
    tree->key = (int*)malloc(sizeof(int) * (order - 1));            //meemory allocation of the key list
    tree->is_leaf = 1;      //initialize the node as leaf
    return tree;
}

void Insert(BNodePtr root, int key) {           //key insertion function
    int median, tmp1, tmp2;
    int* parse_tmp = (int*)malloc(sizeof(int) * root->order);
    BNodePtr tmp_node1, tmp_node2;
    
    if (root->order % 2 == 0)           //evaluate median value
        median = root->order / 2;
    else if (root->order % 2 == 1)
        median = root->order / 2 + 1;

    if (root->size < median  && root->is_leaf == 1) {       //add key in the root when number of keys in root is smaller than median
        if (root->size == 0 || key > root->key[root->size - 1])
            root->key[root->size++] = key;
        else {
            int i;
            for (i = 0; i < root->size && key > root->key[i]; i++)
                continue;
            for (int j = ++root->size; j > i; j--)
                root->key[j] = root->key[j - 1];
            root->key[i] = key;
        }
    }
    else if (root->size >= median && root->is_leaf == 1) {          //add key in the root when number of keys in root is same or bigger than median
        if (root->size < root->order - 1) {         //when number of keys is smaller than the list size
            int i;
            for (i = 0; i < root->size && key > root->key[i]; i++)
                continue;
            for (int j = ++root->size; j > i; j--)
                root->key[j] = root->key[j - 1];
            root->key[i] = key;
        }
        else if (root->size == root->order - 1) {          //when number of keys is same as the list size
            for (int i = 0; i < root->size; i++) {          //add key and all values of list to overflow size temporary list with sorting
                if (key > root->key[i])
                    parse_tmp[i] = root->key[i];
                else if (key > root->key[i - 1] && key < root->key[i])
                    parse_tmp[i] = key;
                else if (key < root->key[i])
                    parse_tmp[i + 1] = root->key[i];
            }
            tmp_node1 = CreateTree(root->order);        //to do parsing, make left parse part node
            for (int i = 0; i < median; i++) {          //input left parse values
                tmp_node1->key[i] = parse_tmp[i];
                tmp_node1->size++;
            }
            tmp_node2 = CreateTree(root->order);        //to do parsing, make right parse part node
            for (int i = median + 1; i <= root->order; i++) {           //input right parse values
                tmp_node2->key[i - (median + 1)] = parse_tmp[i];
                tmp_node2->size++;
            }
            root->key[0] = parse_tmp[median];       //input the value of median index to root
            root->size = 1;     //resize the root
            root->child[0] = tmp_node1;         //make parse parts to child node
            root->child[1] = tmp_node2;
            root->is_leaf = 0;
        }
    }
    else if (root->is_leaf == 0) {      //if root is not leaf, input key in child using recursion
        for (int i = 0; i < root->size; i++) {
            if (key < root->key[0])
                Insert(root->child[0], key);
            else if (key > root->key[i - 1] && key < root->key[i])
                Insert(root->child[i], key);
            else if (key > root->key[root->size - 1])
                Insert(root->child[root->size], key);
        }
    }
}

int Find(BNodePtr root, int key) {          //key finding function
    int result;
    if (root == NULL)
        return 0;
    else {              //find the key using recursion
        for (int i = 0; i < root->size; i++) {
            if (key == root->key[i])        //if the key value exists, return 1
                return 1;
            else if (key < root->key[0])
                result = Find(root->child[0], key);
            else if (key > root->key[i-1] && key < root->key[i])
                result = Find(root->child[i], key);
        }
        if (key > root->key[root->size - 1])
            result = Find(root->child[root->size], key);
    }
    return result;
}

void PrintTree(BNodePtr root) {         //tree printing function
    if (root == NULL)
        return ;
    for (int i = 0; i < root->size; i++) {          //print values of tree inorderly using recursion
        PrintTree(root->child[i]);
        fprintf(fout, "%d ", root->key[i]);
    }
    PrintTree(root->child[root->size]);
}

void DeleteTree(BNodePtr root) {        //tree deleting function
    if (root != NULL) {
        for (int i = 0; i <= root->size; i++)
            DeleteTree(root->child[i]);
        free(root);
    }
}
