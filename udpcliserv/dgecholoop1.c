#include	"unp.h"

static void	recvfrom_int(int);
static int	count;

/*/大多数TCP服务器时并发的，而大多数UDP服务器时迭代的
/dg_echo函数时协议无关的，无关的理由是：调用者必须分配一个正确大小的套接字地址结构，
而指向该结构的指针和该结构的大小都必须作为参数传递给dg_echo。
dg_echo绝不查看这个协议相关结构的内容，而时简单地把一个指向该结构的指针传递给recvfrom和sendto。
*/
void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	socklen_t	len;
	char		mesg[MAXLINE];

	//当我们用终端的中断键终止服务器时（相当于向它发送SIGINT信号），服务器会显示所接收到数据报的数目并终止。
	Signal(SIGINT, recvfrom_int);

	//该函数永不终止，因为UDP是一个无连接的协议，它没有像TCP中EOF之类的东西
	for ( ; ; ) 
	{
		len = clilen;
		Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);

		count++;
	}
}

static void recvfrom_int(int signo)
{
	printf("\nreceived %d datagrams\n", count);
	exit(0);
}
