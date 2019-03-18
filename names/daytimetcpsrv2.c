#include	"unp.h"
#include	<time.h>

int main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		len, addrlen;
	char			buff[MAXLINE];
	time_t			ticks;
	struct sockaddr_storage	cliaddr;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		//简单技巧允许我们强制服务器使用某个给定的协议：允许用户作为程序的命令行参数输入一个IP地址或主机名，并把它传递给getaddrinfo。
		//如果输入的时IP地址，那么IPv4的点分十进制数串不同与IPv6的十六进制数串。
		//影响相应的inet_pton的调用
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: daytimetcpsrv2 [ <host> ] <service or port>");

	for ( ; ; ) 
	{
		len = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &len);
		printf("connection from %s\n", Sock_ntop((SA *)&cliaddr, len));

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
