#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ISFULL 1
#define ISEMPTY 2
#define DIVIDEZERO 3

FILE *fin;
FILE *fout;

typedef struct Stack {
    int *key;
    int top;
    int max_stack_size;
} Stack;

Stack* CreateStack(int max);        //function declaration
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);
void DeleteStack(Stack* S);
int IsEmpty(Stack* S);
int IsFull(Stack* S);

void main(int argc, char *argv[]) {     //main function
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Stack* stack;
    char input_str[101];
    int max = 20, i = 0, a, b, result, error_flag = 0;  //declare and initialize variables

    fgets(input_str, 101, fin);     //get inputs from input file to array of characters
    stack = CreateStack(max);       //make stack

    fprintf(fout, "top numbers : ");
    while (input_str[i] != '#') {
        if (input_str[i] == '1' || input_str[i] == '2' || input_str[i] == '3' || input_str[i] == '4' || input_str[i] == '5' || input_str[i] == '6' || input_str[i] == '7' || input_str[i] == '8' || input_str[i] == '9') {      //if input is operands(1~9)
            if (IsFull(stack)) {        //if stack is full
                error_flag = ISFULL;    //it is error
                break;
            }
            else {      //if stack is not full
                Push(stack, (int)input_str[i] - 48);    //change char to int and push to stack
            }
        }
        else if (input_str[i] == '+') {     //if input is operator '+'
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                b = Pop(stack);     //pop one operand from stack
            }
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                a = Pop(stack);     //pop one operand from stack
            }
            Push(stack, a + b);     //push the result to stack
        }
        else if (input_str[i] == '-') { //if input is operator '-'
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                b = Pop(stack);     //pop one operand from stack
            }
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                a = Pop(stack);     //pop one operand from stack
            }
            Push(stack, a - b);     //push the result to stack
        }
        else if (input_str[i] == '*') { //if input is operator '*'
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                b = Pop(stack);     //pop one operand from stack
            }
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                a = Pop(stack);     //pop one operand from stack
            }
            Push(stack, a * b);     //push the result to stack

        }
        else if (input_str[i] == '/') {     //if input is operator '/'
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                b = Pop(stack);     //pop one operand from stack
            }
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                a = Pop(stack);     //pop one operand from stack
            }
            if (b == 0) {      
                error_flag = DIVIDEZERO;       //if is error
                break;
            }
            Push(stack, a / b);     //push the result to stack
        }
        else if (input_str[i] == '%') {     //if input is operator '%'
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                b = Pop(stack);     //pop one operand from stack
            }
            if (IsEmpty(stack)) {       //if stack is empty
                error_flag = ISEMPTY;   //it is error
                break;
            }
            else {
                a = Pop(stack);     //pop one operand from stack
            }
            if (b == 0) {
                error_flag = DIVIDEZERO;        //it is error
                break;
            }
            Push(stack, a % b);     //push the result to stack
        }
        
        int t = Top(stack);
        fprintf(fout, "%d ", t);    //print the top element
        result = t;
    
        i++;
    }
    if (error_flag == ISFULL) {         //print error message if stack is full
        fprintf(fout, "\nerror : invalid postfix expression, stack is full!\n");
    }
    else if (error_flag == ISEMPTY) {   //print error message if stack is empty
        fprintf(fout, "\nerror : invalid postfix expression, stack is empty!\n");
    }
    else if (error_flag == DIVIDEZERO) {        //print error message if number is divided by zero
        fprintf(fout, "\nerror : invalid postfix expression, divide by zero!\n");
    }
    else {
        if(stack->top+1 > 1) {      //print error message if more than one elements are left in stack at the end
            fprintf(fout, "\nerror : invalid postfix expression, %d elements are left!\n", stack->top+1);
        }
        else {      //print the result if there is no error
            fprintf(fout, "\nevaluation result : %d\n", result);
        }
    }   

    fclose(fin);            //close file
    fclose(fout);
    DeleteStack(stack);     //delete the stack and return memory
}

Stack* CreateStack(int max) {       //stack creating function(create and initialize)
    Stack* S;
    S = malloc(sizeof(Stack));
    S->key = malloc(sizeof(int) * max);
    S->top = -1;
    S->max_stack_size = max;
    return S;
}

void Push(Stack* S, int X) {        //push function
    S->key[++(S->top)] = X;
}

int Pop(Stack* S) {         //pop function
    int pop_num = S->key[(S->top)--];
    return pop_num;
}

int Top(Stack* S) {         //top function
    return S->key[S->top];
}

void DeleteStack(Stack* S) {    //stack delete function
    free(S->key);
    free(S);
}

int IsEmpty(Stack* S) {     //empty check function
    return S->top == -1;
}

int IsFull(Stack* S) {      //full check function
    return (S->top) + 1 == S->max_stack_size;
}
