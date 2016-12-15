#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 4 // 修改1
#define EPOLL_SIZE 50

void error_handling(char *msg);

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	struct epoll_event * ep_events;
	struct epoll_event event;
	int epfd, event_cnt;

	if (argc != 2){
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}


	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");

	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");	

	
	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	event.events = EPOLLIN; // EPOLLIN:需要读取数据的情况
	//event.events = EPOLLIN | EPOLLET; // EPOLLET:以边缘触发的方式得到事件通知
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	int cnt = 0;
	while(1)
	{
		event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		
		if (event_cnt == -1)
		{
			puts("epoll_wait() error");
			break;
		}
		printf("-----epoll_wait----%d\n", ++cnt); // 修改2
		// 条件触发下，当在输入缓冲收到数据的情况下，如果不读取（一次读取不完或者延迟处理），则每次调用epoll_wait函数都会产生相应的事件，这是不合理的。
		// 当改小了缓冲大小，阻止服务器一次性读完接收的数据时，并且以条件触发（默认）的方式。
		// epoll_wait将多次输出
		// 使用边缘触发 EPOLLET时，只输出一次，和设置足够大的的缓冲时一样。但是，只能回复给客户端4个字节(设置的缓冲大小)的数据。

		for (i=0; i<event_cnt; i++)
		{
			if (ep_events[i].data.fd == serv_sock)
			{
				adr_sz = sizeof(clnt_adr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
				event.events = EPOLLIN; // | EPOLLET;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("connected client:%d\n", clnt_sock);
			}
			else
			{
				str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
				if (str_len == 0) // close
				{
					epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client:%d\n", ep_events[i].data.fd);
				}
				else // echo
				{
					write(ep_events[i].data.fd, buf, str_len);
				}
			}
		}
	}
	
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
