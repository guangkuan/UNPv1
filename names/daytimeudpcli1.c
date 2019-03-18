#include	"unp.h"

int main(int argc, char **argv)
{
	int				sockfd, n;
	char			recvline[MAXLINE + 1];
	socklen_t		salen;
	struct sockaddr	*sa;

	if (argc != 3)
		err_quit("usage: daytimeudpcli1 <hostname/IPaddress> <service/port#>");

	//调用udp_client函数，然后显示将向其发送UDP数据报的服务器的IP地址和端口号
	sockfd = Udp_client(argv[1], argv[2], (void **) &sa, &salen);

	printf("sending to %s\n", Sock_ntop_host(sa, salen));

	//发送1字节的数据报
	Sendto(sockfd, "", 1, 0, sa, salen);	/* send 1-byte datagram */

	n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
	recvline[n] = '\0';	/* null terminate */
	Fputs(recvline, stdout);

	exit(0);
}
