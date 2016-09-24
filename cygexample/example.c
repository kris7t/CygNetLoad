
#include "example.h"

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void say_hello(char *name)
{
	char buf[1024];
	int length = sprintf(buf, "Hello %s!", name);
	/*int fd = open("hello.txt", O_CREAT | O_WRONLY | O_TRUNC);
	write(fd, buf, length);
	close(fd);*/
	FILE *f = fopen("hello.txt", "w");
	fwrite(buf, sizeof(char), length, f);
	fclose(f);
}
