#include	"unp.h"
#include	<time.h>

int main(int argc, char **argv)
{
	int				listenfd, connfd;
	socklen_t		len;
	char			buff[MAXLINE];
	time_t			ticks;
	struct sockaddr_storage	cliaddr;

	if (argc != 2)
		err_quit("usage: daytimetcpsrv1 <service or port#>");

	//需要一个命令行参数来指定服务名或端口号
	//第三个参数是一个空指针，因为我们不关心当前地址族在使用多大大小的地址结构
	listenfd = Tcp_listen(NULL, argv[1], NULL);

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
