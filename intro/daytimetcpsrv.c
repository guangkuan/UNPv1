#include	"unp.h"
#include	<time.h>

int main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(13);	/* daytime server */

	/*/bind函数把一个本地协议地址赋予一个套接字
	/第二个参数是一个指向特定于协议的地址结构的指针，第三个参数是该地址结构的长度
	/如果制定端口号为0，那么内核就在bind被调用时选择一个临时端口。然而如果指定IP地址为通配地址，那么内核将等到套接字已连接（TCP）或已在套接字上发出数据报（UDP）时选择一个本地IP地址
	*/
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	/*/listen函数把一个未连接的套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求
	/第二个参数规定了内核应该为相应套接字排队的最大连接个数（未完成连接队列 + 已完成连接队列 <= 第二个参数）
	/本函数通常应该在调用socket和bind这两个函数之后，并在调用accept函数之前调用
	*/
	Listen(listenfd, LISTENQ);

	for ( ; ; ) 
	{
		/*accept函数由TCP服务器调用，用于从已完成连接队列队头返回下一个已完成连接，如果已完成连接队列为空，那么进程被投入睡眠
		/如果accept成功，那么其返回值是由内核自动生成的一个全新描述符(已连接套接字描述符)，代表与所返回客户的TCP连接
		/如果我们对返回客户协议地址不感兴趣，那么可以把cliaddr和addrlen均置为空指针
		*/
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
