#include "buffer.h"

int main(int argc, char **argv)
{
	char line [] = "Hello World!"; //Expected 'line_len': 12
	struct BUF_buffer buffer;

	BUF_init(&buffer);
	BUF_puts(&buffer, line);

	BUF_print_all(&buffer);
	printf("BUF_line_len(): %d\t(expected 12)\n", BUF_line_len(&buffer));
	return 0;
}
	


