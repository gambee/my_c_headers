/* buffer.h
 *
 * Max Gambee
 * Copyright 2017
 *
 * Description: Buffer data structure for i/o in C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef BUF_SIZE
#define BUF_SIZE 10
#endif

struct BUF_node
{
	char data[BUF_SIZE];
	struct BUF_node* next;
};

struct BUF_buffer
{
	unsigned int front_index, back_index;
	struct BUF_node *front, *back;
};

int BUF_append_node(struct BUF_buffer *buf)
{
	struct BUF_node *tmp;
	if(!buf)
		return 1; //NULL ptr passed
	
	tmp = (struct BUF_node*) malloc(sizeof(struct BUF_node));
	tmp->next = NULL;
	buf->back_index = 0;

	buf->back = buf->back ? buf->back->next = tmp : tmp;
	return 0;
}

void BUF_init(struct BUF_buffer *to_init)
{
	to_init->front_index = 0;
	to_init->back_index = 0;
	to_init->front = NULL;
	to_init->back = NULL;
}

int BUF_line_len(struct BUF_buffer *buf)
{
	/* returns number of characters until, but not including, the next
	 * end-of-line character, null-terminating character, or back of stream.
	 * This integer is positive if the end-of-line character is reached, and
	 * negative in the other two cases. It returns zero if the line len is zero
	 * or if buf is a NULL pointer.
	 */

	struct BUF_node *cur;
	int i, count = 0;

	if(!buf)
		return 0; //exit immediately if invalid NULL pointer is passed

	cur = buf->front;
	if(cur == buf->back)
		for(i = 0; i < buf->back_index && cur->data[i] != '\n' && cur->data[i]; i++, count++);
	else
	{
		for(i = buf->front_index; i < BUF_SIZE && cur->data[i] != '\n' && cur->data[i]; i++, count++);
		for(cur=cur->next; cur != buf->back; cur=cur->next)
			for(i = 0; i < BUF_SIZE && cur->data[i] != '\n' && cur->data[i]; i++, count++);
		for(i = 0; i < buf->back_index && cur->data[i] != '\n' && cur->data[i]; i++, count++);

	}
	return (cur->data[i] == '\n') ? count : -count;
}

int BUF_getc(struct BUF_buffer *buf)
{
	struct BUF_node *tmp;
	int ret = 0;
	
	if(buf && buf->front)
	{
		if(buf->front == buf->back)
		{
			if(buf->front_index < buf->back_index && buf->front_index < BUF_SIZE)
				ret = buf->front->data[buf->front_index++];
		}
		else
		{
			if(buf->front_index < BUF_SIZE)
				ret = buf->front->data[buf->front_index++];
			else
			{
				tmp = buf->front;
				buf->front = buf->front->next;
				buf->front_index = 0;
				ret = buf->front ? buf->front->data[buf->front_index++] : 0;
				free(tmp);
			}
		}
	}

	return ret;
}

/*
int BUF_puts(struct BUF_buffer *buf, char *str)
{
	if(!buf)
		return 1; //NULL ptr passed

	if(buf->back)
	{
		if(buf->back_index < BUF_SIZE)
		{
			buf->back->data[buf->back_index++] = c;
		}
		else
		{
			buf->back->next = (struct BUF_node*) malloc(sizeof(struct BUF_node));
			buf->back = buf->back->next;
			buf->back->next = NULL;
			buf->back->data[0] = c;
			buf->back_index = 1;
			++ret;
		}
	}
	else
	{
		buf->back = (struct BUF_node*) malloc(sizeof(struct BUF_node));
		buf->front = buf->back;
		buf->front_index = 0;
		buf->back->next = NULL;
		buf->back->data[0] = c;
		buf->back_index = 1;
		++ret;
	}

	return 0;
}
*/

int BUF_putc(struct BUF_buffer *buf, char c)
{
	/* returns zero on success */

	if(!buf)
		return -1; // NULL ptr passed; exit in error immediately

	if(buf->back)
	{
		if(buf->back_index < BUF_SIZE)
		{
			buf->back->data[buf->back_index++] = c;
		}
		else
		{
			buf->back->next = (struct BUF_node*) malloc(sizeof(struct BUF_node));
			buf->back = buf->back->next;
			buf->back->next = NULL;
			buf->back->data[0] = c;
			buf->back_index = 1;
		}
	}
	else
	{
		buf->back = (struct BUF_node*) malloc(sizeof(struct BUF_node));
		buf->front = buf->back;
		buf->front_index = 0;
		buf->back->next = NULL;
		buf->back->data[0] = c;
		buf->back_index = 1;
	}

	return 0; //success!
}

int BUF_print_all(struct BUF_buffer *buf)
{
	struct BUF_node *cur = NULL;
	struct BUF_node *back = NULL;
	int chars = 0;
	int nodes = 0;
	int i;

	if(buf)
	{
		back = buf->back;
		cur = buf->front;
		if(cur && buf->front_index < BUF_SIZE && buf->back_index <= BUF_SIZE)
		{
			if(cur == back)
			{
				printf("Node %d:\n", nodes++);
				for(i = 0; i <= buf->front_index && i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i == buf->front_index ? "<----Front Element" : "");
			
				for(NULL; i <= buf->back_index && i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i + 1 == buf->back_index ? "<----Back Element" : "");
				for(NULL; i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d\n", i
					, isprint(cur->data[i]) ? cur->data[i] : '@'
					, cur->data[i]);
			}
			else
			{
				/* front node: */
				printf("Node %d:\n", nodes++);
				for(i = 0; i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i == buf->front_index ? "<----Front Element" : "");

				/* set up for tail iteration */
				cur = cur->next;
				printf("Node %d:\n", nodes++);

				/* middle nodes: */
				while(cur && cur != buf->back)
				{
					for(i = 0; i < BUF_SIZE; i++)
						printf("\tdata[%d] : %c : %d\n", i
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]);
					cur = cur->next;
					printf("Node %d:\n", nodes++);
				}

				/* back node: */
				for(i = 0; i < BUF_SIZE; i++)
					printf("\tdata[%d] : %c : %d %s\n", i 
						, isprint(cur->data[i]) ? cur->data[i] : '@'
						, cur->data[i]
						, i + 1 == buf->back_index ? "<----Back Element" : "");
			}
		}
		else
		{
			return -1;
		}
	}
	else 
	{
		return -2;
	}

	return 0;
}






