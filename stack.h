

/*
 * Type: stackElementT
 * -------------------
 * This is the type of the objects entered in the stack.
 * Edit it to change the type of things to be placed in
 * the stack.
 */

typedef struct command * commandStackElement;
//typedef enum command_type * opStackElement;

typedef struct operator{
	enum command_type * op_type;
	int value;
} * opStackElement;

/*
 * Type: stackT
 * --------------
 * This is the type for a stack, i.e., it is a type that
 * holds the information necessary to keep track of a stack.
 * It has a pointer `contents' to a dynamically-allocated
 * array (used to hold the contents of the stack), an integer
 * `maxSize' that holds the size of this array (i.e., the
 * maximum number of things that can be held in the stack),
 * and another integer `top,' which stores the array index of
 * the element at the top of the stack.
 */

typedef struct {
  commandStackElement *contents;
  int maxSize;
  int top;
} commandStack;

typedef struct {
  opStackElement *contents;
  int maxSize;
  int top;
} opStack;

/*
 * Function: StackInit
 * Usage: StackInit(&stack, maxSize);
 * -------------------------
 * A new stack variable is initialized.  The initialized
 * stack is made empty.  MaxSize is used to determine the
 * maximum number of character that can be held in the
 * stack.
 */

void CommandStackInit(commandStack *stackP, int maxSize);
void OpStackInit(opStack *stackP, int maxSize);

/* Function: StackDestroy
 * Usage: StackDestroy(&stack);
 * -----------------------
 * This function frees all memory associated with the stack.
 * The `stack' variable may not be used again unless
 * StackInit(&stack, maxSize) is first called on the stack.
 */

void CommandStackDestroy(commandStack *stackP);
void OpStackDestroy(opStack *stackP);

/*
 * Functions: StackPush, StackPop
 * Usage: StackPush(&stack, element); element = StackPop(&stack);
 * --------------------------------------------
 * These are the fundamental stack operations that add an element to
 * the top of the stack and remove an element from the top of the stack.
 * A call to StackPop on an empty stack or to StackPush on a full stack
 * is an error.  Make use of StackIsEmpty()/StackIsFull() (see below)
 * to avoid these errors.
 */

void CommandStackPush(commandStack *stackP, commandStackElement element);
void OpStackPush(opStack *stackP, opStackElement element);
commandStackElement CommandStackPop(commandStack *stackP);
opStackElement OpStackPop(opStack *stackP);

commandStackElement CommandStackTop(commandStack *stackP);
opStackElement OpStackTop(opStack *stackP);

/*
 * Functions: StackIsEmpty, StackIsFull
 * Usage: if (StackIsEmpty(&stack)) ...
 * -----------------------------------
 * These return a true value if the stack is empty
 * or full (respectively).
 */

int CommandStackIsEmpty(commandStack *stackP);
int OpStackIsEmpty(opStack *stackP);
int CommandStackIsFull(commandStack *stackP);
int OpStackIsFull(opStack *stackP);
