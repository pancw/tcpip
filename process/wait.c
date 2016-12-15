#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	// WIFEXITED 子进程正常终止时返回true
	// WEXITSTATUS 返回子进程的返回值

	int status;
	pid_t pid = fork();

	if (pid == 0)
	{
		return 3;
	}
	else
	{
		printf("Child PID: %d \n", pid);
		pid = fork();
		if (pid == 0)
		{
			exit(7);
		}
		else
		{
			printf("Child PID: %d \n", pid);
			wait(&status); // 如果没有子进程终止， 会阻塞
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			wait(&status); // 调用wait之后，子进程会完全被销毁
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			sleep(30);
		}
	}
	return 0;
}
