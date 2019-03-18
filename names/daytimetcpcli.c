#include	"unp.h"

int main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		len;
	//sockaddr_storage套接字地址结构，它大的足以存放系统支持的任何套接字地址类型，又能满足它们的对齐限制
	struct sockaddr_storage	ss;

	if (argc != 3)
		err_quit("usage: daytimetcpcli <hostname/IPaddress> <service/port#>");

	//用命令行参数指定IP地址、服务名或端口号
	sockfd = Tcp_connect(argv[1], argv[2]);

	len = sizeof(ss);
	//调用getpeername取得服务器的协议地址并显示出来。
	Getpeername(sockfd, (SA *)&ss, &len);
	printf("connected to %s\n", Sock_ntop_host((SA *)&ss, len));

	while ( (n = Read(sockfd, recvline, MAXLINE)) > 0) 
	{
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	exit(0);
}
