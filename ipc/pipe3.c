#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 30

int main()
{
	int fds1[2], fds2[2];
	char str1[] = "hello world.";
	char str2[] = "thanks.";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1), pipe(fds2);
	pid = fork();

	if (pid == 0)
	{
		write(fds1[1], str1, sizeof(str1));
		read(fds2[0], buf, BUF_SIZE); // 会阻塞
		printf("Child proc output:%s\n", buf);
	}
	else
	{
		read(fds1[0], buf, BUF_SIZE);
		printf("Parent proc output:%s\n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3); // 不能少
	}
	return 0;
}
