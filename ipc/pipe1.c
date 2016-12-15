#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main()
{
	int fds[2];
	char str[] = "hello world.";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds); // 复制的并非管道， 而是文件描述符
	pid = fork();
	
	if (pid == 0)
	{
		write(fds[1], str, sizeof(str));
	}
	else
	{
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}

	return 0;
}
