#include "stack.h"


#include <error.h>
#include <errno.h>
#include <stdlib.h>


void CommandStackInit(commandStack *stackP, int maxSize)
{
  commandStackElement *newContents;

  /* Allocate a new array to hold the contents. */

  newContents = (commandStackElement *)malloc(sizeof(commandStackElement) * maxSize);


  stackP->contents = newContents;
  stackP->maxSize = maxSize;
  stackP->top = -1;  /* I.e., empty */
}

void OpStackInit(opStack *stackP, int maxSize)
{
  opStackElement *newContents;

  /* Allocate a new array to hold the contents. */

  newContents = (opStackElement *)malloc(sizeof(opStackElement) * maxSize);

  stackP->contents = newContents;
  stackP->maxSize = maxSize;
  stackP->top = -1;  /* I.e., empty */
}

void CommandStackDestroy(commandStack *stackP)
{
  /* Get rid of array. */
  free(stackP->contents);

  stackP->contents = NULL;
  stackP->maxSize = 0;
  stackP->top = -1;  /* I.e., empty */
}

void OpStackDestroy(opStack *stackP)
{
  /* Get rid of array. */
  free(stackP->contents);

  stackP->contents = NULL;
  stackP->maxSize = 0;
  stackP->top = -1;  /* I.e., empty */
}

void CommandStackPush(commandStack *stackP, commandStackElement element)
{
 

  /* Put information in array; update top. */

  stackP->contents[++stackP->top] = element;
}

void OpStackPush(opStack *stackP, opStackElement element)
{

  /* Put information in array; update top. */

  stackP->contents[++stackP->top] = element;
}

commandStackElement CommandStackPop(commandStack *stackP)
{
  if (CommandStackIsEmpty(stackP))
    error(1,0,"stack is empty");

  return stackP->contents[stackP->top--];
}

opStackElement OpStackPop(opStack *stackP)
{
  if (OpStackIsEmpty(stackP))
    error(1,0,"stack is empty");

  return stackP->contents[stackP->top--];
}


int CommandStackIsEmpty(commandStack *stackP)
{
  return stackP->top < 0;
}

int OpStackIsEmpty(opStack *stackP)
{
  return stackP->top < 0;
}


int CommandStackIsFull(commandStack *stackP)
{
  return stackP->top >= stackP->maxSize - 1;
}

int OpStackIsFull(opStack *stackP)
{
  return stackP->top >= stackP->maxSize - 1;
}