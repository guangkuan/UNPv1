#include	"unp.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int				n;
	char			sendline[MAXLINE], recvline[MAXLINE + 1];
	socklen_t		len;
	struct sockaddr	*preply_addr;

	preply_addr = Malloc(servlen);

	while (Fgets(sendline, MAXLINE, fp) != NULL) 
	{
		/*/假设客户没有捆绑一个具体的IP地址到其套接字上，客户的IP地址将随客户发送的每个UDP数据报而动
		/对于一个UDP套接字，如果其进程首次调用sendto时，它没有绑定一个本地端口，那么内核就在此时为它选择一个临时端口。*/
		Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);

		len = servlen;
		//在recvfrom的调用中，我们通知内核返回数据报发送者的地址。
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
		//我们首先比较由recvfrom在值-结果参数中返回的长度，然后用memcmp比较套接字地址结构本身。
		if (len != servlen || memcmp(pservaddr, preply_addr, len) != 0) 
		{
			printf("reply from %s (ignored)\n", Sock_ntop(preply_addr, len));
			continue;
		}

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}
