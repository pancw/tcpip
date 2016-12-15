#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 1024

int main()
{
	fd_set reads, temps;
	int result, str_len;
	char buf[BUF_SIZE];
	struct timeval timeout;
	
	FD_ZERO(&reads);
	FD_SET(STDIN_FILENO, &reads); // 0

	while (1)
	{
		//需要copy，因为调用select之后除了发生
		//变化的文件描述对应位外，剩下的位将初
		//始化为0
		temps = reads;

		// 调用select之后， tv_sec和tv_usec的值
		// 会被替换为超时前剩余的时间
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;	
	
		// 1最大描述符+1
		result = select(1, &temps, 0, 0, &timeout);
		if (result == -1)
		{
			puts("select() error!");
			break;
		}
		else if (result == 0)
		{
			puts("Time out!");
		}
		else
		{
			if (FD_ISSET(STDIN_FILENO, &temps))
			{
				str_len = read(STDIN_FILENO, buf, BUF_SIZE);
				buf[str_len] = 0;
				printf("message from console:%s", buf);
			}
		}
	}
	return 0;
}
