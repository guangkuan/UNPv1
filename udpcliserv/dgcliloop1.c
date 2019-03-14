#include	"unp.h"

#define	NDG		2000	/* datagrams to send */
#define	DGLEN	1400	/* length of each datagram */

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int		i;
	char	sendline[DGLEN];

	//发送2000个1400字节大小的UDP数据报给服务器
	for (i = 0; i < NDG; i++) 
	{
		Sendto(sockfd, sendline, DGLEN, 0, pservaddr, servlen);
	}
}
