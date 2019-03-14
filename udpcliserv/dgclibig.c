#include	"unp.h"

#undef	MAXLINE
#define	MAXLINE 65507

void
dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int			size;
	char		sendline[MAXLINE], recvline[MAXLINE + 1];
	ssize_t		n;

	size = 70000;
	Setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
	Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

	Sendto(sockfd, sendline, MAXLINE, 0, pservaddr, servlen);
	//后两个参数时空指针，这告知内核我们并不关心应答数据报由谁发送
	n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);

	printf("received %d bytes\n", n);
}
