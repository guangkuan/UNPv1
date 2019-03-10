#include	"unp.h"

/*/在批量方式下，标准输入中的EOF并不意味着我们同时也完成了从套接字的读入；
可能仍有请求在去往服务器的路上，或者仍由应答在返回客户的路上。
/我们需要一种关闭TCP连接其中一半的方法。
也就是说，我们想给服务器发送一个FIN，告诉它我们已经完成了数据发送，但是仍然保持套接字描述符打开以便读取。
*/
void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof;
	fd_set		rset;
	char		buf[MAXLINE];
	int		n;

	stdineof = 0;
	FD_ZERO(&rset);
	for ( ; ; ) 
	{
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) 
		{	/* socket is readable */
			if ( (n = Read(sockfd, buf, MAXLINE)) == 0) 
			{
				if (stdineof == 1)
					return;		/* normal termination */
				else
					err_quit("str_cli: server terminated prematurely");
			}

			/*write对缓冲区而不是文本行进行操作，消除为提升性能而引入缓冲机制增加了网络应用程序的复杂性问题。*/
			Write(fileno(stdout), buf, n);
		}

		if (FD_ISSET(fileno(fp), &rset)) 
		{  /* input is readable */
			/*改用read对缓冲区而不是文本行进行操作*/
			if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) 
			{
				stdineof = 1;
				/*close把描述符的引用计数减1，仅在该计数变为0时才关闭套接字。使用shutdown可以不管引用计数就激发TCP的正常连接终止序列；
				close终止读和写两个方向的数据传送。shutdown的行为由howto参数指定。*/
				Shutdown(sockfd, SHUT_WR);	/* send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			Writen(sockfd, buf, n);
		}
	}
}
