#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	// WIFEXITED �ӽ���������ֹʱ����true
	// WEXITSTATUS �����ӽ��̵ķ���ֵ

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
			wait(&status); // ���û���ӽ�����ֹ�� ������
			if (WIFEXITED(status))
				printf("Child send one: %d \n", WEXITSTATUS(status));

			wait(&status); // ����wait֮���ӽ��̻���ȫ������
			if (WIFEXITED(status))
				printf("Child send two: %d \n", WEXITSTATUS(status));

			sleep(30);
		}
	}
	return 0;
}
