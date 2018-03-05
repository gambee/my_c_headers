/* strqueue.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef STRQUEUE_H
#	define STRQUEUE_H

#ifdef STRQUEUE_DEBUG
#	define Q_NODE_SIZE 16
#else
#	define Q_NODE_SIZE 512
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Structures:
 */

struct que_node
{
	struct que_node* next;
	char* strings[Q_NODE_SIZE];
};

struct strqueue
{
	struct que_node *front, *back;
	int front_index, back_index;
};

#define strqueueinit(X) (X.back = NULL)

struct que_node* mk_que_node(struct que_node* next)
{
	struct que_node* ret = (struct que_node*) malloc(sizeof(struct que_node));
	if(ret)
	{
		ret->next = next;
		memset(ret->strings, 0, Q_NODE_SIZE);
	}
	return ret;
}

int str_enqueue(struct strqueue* queue, char* str)
{
	char* tmp;

	if(!str)
		return -1; //malloc fail
	
	if(queue->back)
	{
			//if back node is full, add another
		if(queue->back_index == Q_NODE_SIZE)
		{
			if((queue->back->next = mk_que_node(NULL)) == NULL)
				return -2; //malloc fail

				//reset pts & indices
			queue->back = queue->back->next;
			queue->back_index = 0;
		}
	}
	else
	{
			//empty queue, add first node, and update all indices/ptrs
		if((queue->back = mk_que_node(NULL)) == NULL)
			return -2; //malloc fail

		queue->front = queue->back;
		queue->front_index = 0;
		queue->back_index = 0;
	}
		
	if((tmp = (char*)malloc(strlen(str)+1)) == NULL)
		return -2; //malloc fail

	strcpy(tmp, str);
	queue->back->strings[queue->back_index++] = tmp;

	return 0;
}

/* Function: str_dequeue
 * if successful, copys the front string in the queue into *dest, 
 * allocating a large enough string to hold it in the case that
 * *dest is NULL (programmer should free when done).
 *
 * returns length of string if successful (0 length means empty queue)
 * and negative on error
 */
int str_dequeue(struct strqueue* queue, char** dest)
{
	struct que_node* tmp;

	if(!queue)
		return -1; 
	
	if(!queue->front)
		return 0; //empty queue, return 0;

	if(!queue->front->strings[queue->front_index])
		return -3; //oops corrupted queue

	if(*dest == NULL)
		if((*dest = (char*)malloc(strlen(queue->front->strings[queue->front_index] + 1))) == NULL)
			return -2; //malloc fail

	//now we assume we have a valid *dest char[] allocated, and a valid entry to dequeue
	strcpy(*dest, queue->front->strings[queue->front_index]); //dequeue and store in *dest
	free(queue->front->strings[queue->front_index++]);	//free the dequeued entry, and update index

	if(queue->front_index == Q_NODE_SIZE) //front node empty! free it..
	{
		if(queue->front != queue->back)
		{
			tmp = queue->front;
			queue->front = tmp->next;
			queue->front_index = 0;
			free(tmp);
		}
		else 
		{
			free(queue->front);
			queue
	
	

int str_print_queue(struct strqueue* queue)
{
	struct que_node *cur, *back;
	int index, nodes, ret = 0;

	if(!queue)
		printf("Empty Queue...\n");
	else
	{
		nodes = 0;

		if((back = queue->back) != queue->front)
		{
			printf("Node %d:\n", nodes++);

		
			for(index = queue->front_index, cur = queue->front;
				index < Q_NODE_SIZE;
				++index, ++ret)
			{
				printf("\t%d: %s\n"
					,index
					,cur->strings[index] == NULL ? "NULL" : cur->strings[index]);
			}
		}
		else
		{
			printf("Node %d:\n", nodes++);

		
			for(index = queue->front_index, cur = queue->front;
				index < queue->back_index;
				++index, ++ret)
			{
				printf("\t%d: %s\n"
					,index
					,cur->strings[index] == NULL ? "NULL" : cur->strings[index]);
			}

			printf("Total Nodes: %d\nTotal Strings:%d\n\n", nodes, ret);
			return ret;
		}

		for(cur = cur->next;
			cur != back;
			cur = cur->next)
		{
			printf("Node %d:\n", nodes++);

			for(index = 0; index < Q_NODE_SIZE; ++index, ++ret)
			{
				printf("\t%d: %s\n"
					,index
					,cur->strings[index] == NULL ? "NULL" : cur->strings[index]);
			}
		}

		printf("Node %d:\n", nodes++);

		for(index = 0;
			index < queue->back_index;
			++index, ++ret)
		{
			printf("\t%d: %s\n"
				,index
				,back->strings[index] == NULL ? "NULL" : back->strings[index]);
		}
	}

	printf("Total Nodes: %d\nTotal Strings:%d\n\n", nodes, ret);
	return ret;
}

#endif
