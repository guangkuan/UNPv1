#include	"unp.h"
//为解决异步错误不会返回到UDP套接字的问题调用connect
void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int		n;
	char	sendline[MAXLINE], recvline[MAXLINE + 1];
	/*没有三次握手过程，内核只是检查是否存在立即可知的错误（例如一个显然不可达的目的地），
	记录对端的IP地址和端口号（取自传递给connect的套接字地址结构），然后立即返回到调用进程*/
	Connect(sockfd, (SA *) pservaddr, servlen);

	while (Fgets(sendline, MAXLINE, fp) != NULL) 
	{
		//对于已连接UDP套接字，与默认的未连接UDP套接字相比，我们再也不能给输出操作指定目的IP地址和端口号
		//也就是说，我们不适用sendto而改用write或send。写到已连接UDP套接字上的任何内容都自动发送到由connect指定的协议地址（如IP地址和端口号）
		Write(sockfd, sendline, strlen(sendline));

		//不必使用recvfrom已获悉数据报的发送者，而改用read、recv或recvmsg。
		//由已连接UDP套接字引发的异步错误会返回给它们所在的进程，而未连接UDP套接字不接受任何异步错误。
		n = Read(sockfd, recvline, MAXLINE);

		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
}
