/* char_stack.h
 *
 * Max Gambee
 * Copyright 2017
 *
 * Description: A simple stack implementation for char elements.
 *  Originally intended for use by the sentential logic expression evaluator
 *  of fli, but can by used as a generic byte size stack, (and easily modified
 *  to other sizes of elements).
 */

#ifndef FLI_STACK_H
#	define FLI_STACK_H

#include <stdlib.h>		//needed for memory mngmnt (i.e. (m)alloc)


/* Constant: STK_SIZE
 * 		Description: This constant specifies the size of the array in each
 * 			node of the  Linearly-Linked-Array implementation of the stack.
 */
#ifdef DEBUG
#	ifndef STK_SIZE
#	define STK_SIZE 8
#	endif
#else
#	ifndef STK_SIZE
#	define STK_SIZE 1016 //sizeof node should be 1024 on x64
#	endif
#endif

/* Macro: STK_mknode
 * 		Description: Takes a pointer to STK_node as an argument, and attempts
 * 			to (m)allocate memory for a STK_node and initialize its next
 * 			pointer to NULL.
 * 		Arguments(s): 	1 : x - Pointer to a STK_node.
 *		Returns:	 0 : Success.
 *					-1 : Failed to allocate node.
 */
#ifndef STK_mknode
#	define STK_mknode(x) ((x = (struct STK_node *) \
		malloc(sizeof(struct STK_node))) ? (x->next = NULL, 0) : -1)
#endif

/* Macro: STK_init
 *		Description: Initializes the stack passed to it to zero/NULL members.
 *		Argument(s):	1 : to_init - The stack to initialize.
 *		Returns:	<no return>
 */
#define STK_init(to_init) ((to_init).top = NULL, (to_init).index = 0)

/* Structure: STK_node
 * 		Description: A node in the Linear-Linked-Array implementation of the 
 * 			stack. Next pointer appears first for structure packing reasons.
 */
struct STK_node{
	struct STK_node *next;
	char data[STK_SIZE];
};

/* Structure: STK_stack
 * 		Description: Encapsulates the stack data structure, so that the
 * 			application programmer need not deal with pointers indices, etc.
 */
struct STK_stack{
	struct STK_node *top;
	int index;
};

/* Function: STK_pop
 * 		Description: Attempts to pop a character from the stack and store it
 * 			in the memory location pointer to by dest. The stack argument 
 * 			should have at some point been initialized via a call to STK_init.
 * 		Argument(s):	1 : stk - a pointer to an initialized stack structure.
 * 						2 : dest - where the popped character will be stored.
 * 		Returns:	0 : Success!
 * 				   -1 : Empty stack.
 * 				   -2 : stk == (nil)
 */
int STK_pop(struct STK_stack *stk, char *dest)
{
	struct STK_node *tmp;

	if(stk)
	{
		if(stk->top ? (stk->index ? 1 : (stk->top->next == NULL ? 0 : 1)) : 0)
		{
			if(!stk->index)
			{
				tmp = stk->top;
				stk->top = stk->top->next;
				stk->index = STK_SIZE;
				free(tmp);
			}
			*dest = stk->top->data[ --stk->index ];
			return 0;
		}
		else return -1; //empty stack
	}
	else return -2; //stk == (nil)
}


/* Function: STK_push
 * 		Description: 
 */
int STK_push(struct STK_stack *stk, char from)
{
	struct STK_node *tmp;

	if(stk)
	{
		if(!stk->top)
		{
			if(STK_mknode(tmp))
				return -1;

			stk->top = tmp;
			stk->index = 0;
		}
		else if(stk->index >= STK_SIZE)
		{
			if(STK_mknode(tmp))
				return -1;

			tmp->next = stk->top;
			stk->top = tmp;
			stk->index = 0;
		}

		stk->top->data[ stk->index++ ] = from;

		return 0;
	}
	else return -2; //stk == (nil)
}


#ifdef DEBUG

#include <stdio.h>

int STK_printall(struct STK_stack *stk)
{
	int i, j = 0;
	struct STK_node *cur = stk ? stk->top : NULL;

	if(cur)
	{
		i = stk->index - 1;
		do{
			printf("Node : %d\n", j);
			if(i + 1)
			{
				do{
					printf("\t[%d] : %c\n", i, cur->data[i]);
				}while(i--);
			}
		}while(i = STK_SIZE - 1, ++j, cur = cur->next);
	}
	else return -1;
}
#endif //DEBUG

#endif //FLI_STACK_H
