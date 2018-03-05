#include <stdio.h>
#include "../fstring/fstring.h"
#define STRQUEUE_DEBUG
#include "strqueue.h"

int main(void)
{
	int cont = 1;
	struct strqueue queue;
	char input[500];

	strqueueinit(queue);

	printf("Enqueue as many strings as you like, and enter 'quit' when done\n");
	printf("enqueue string: ");
	getsline(input);

	while(cont)
	{
		while(strcmp(input, "quit"))
		{
			str_enqueue(&queue, input);
			printf("enqueue string: ");
			getsline(input);
		}

		printf("Showing Contents of Queue:\n\n");
		str_print_queue(&queue);

		printf("\n\t again? (yes/quit): ");
		getsline(input);

		if(strcmp(input, "quit"))
		{
			printf("enqueue string: ");
			getsline(input);
		}
		else cont = 0;
	}

	return 0;
}
	
	



