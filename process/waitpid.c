#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	int status;
	pid_t pid = fork();

	if (pid == 0)
	{
		sleep(15);
		return 24;
	}
	else
	{
		// pid_t waitpid(pid_t pid, int * statloc, int options);
		// pid: 若传-1， 则与wait函数相同, 可以等待任意子进程终止
		// options: 传递头文件sys/wait.h 中声明的常量WNOHANG 不会阻塞
		while (!waitpid(pid, &status, WNOHANG))
		{
			sleep(3);
			puts("sleep 3 sec.");
		}

		if (WIFEXITED(status))
			printf("Child send %d \n", WEXITSTATUS(status));
	}
	return 0;
}
