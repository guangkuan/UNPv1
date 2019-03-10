/* include fig01 */
#include	"unp.h"

int main(int argc, char **argv)
{
	int					i, maxi, maxfd, listenfd, connfd, sockfd;
	int					nready, client[FD_SETSIZE];
	ssize_t				n;
	fd_set				rset, allset;
	char				buf[MAXLINE];
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	maxfd = listenfd;			/* initialize */
	maxi = -1;					/* index into client[] array */
	//整型数组client含有每个客户的已连接套接字描述符，所有元素被初始化为-1
	for (i = 0; i < FD_SETSIZE; i++)
		client[i] = -1;			/* -1 indicates available entry */
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
/* end fig01 */

/* include fig02 */
	for ( ; ; ) 
	{
		//拷贝不修改原结构体
		rset = allset;		/* structure assignment */
		//select等待某个事件发生：或是新客户连接的建立，或是数据、FIN或RST的到达
		nready = Select(maxfd+1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(listenfd, &rset)) 
		{	/* new client connection */
			clilen = sizeof(cliaddr);
			/*如果监听套接字变为可读（就绪），那么已建立了一个新的连接。
			我们调用accept并相应地更新数据结构
			*/
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s, port %d\n",
					Inet_ntop(AF_INET, &cliaddr.sin_addr, 4, NULL),
					ntohs(cliaddr.sin_port));
#endif

			//使用client数组中的第一个未用项记录这个已连接描述符
			for (i = 0; i < FD_SETSIZE; i++)
				if (client[i] < 0) 
				{
					client[i] = connfd;	/* save descriptor */
					break;
				}
			if (i == FD_SETSIZE)
				err_quit("too many clients");

			FD_SET(connfd, &allset);	/* add new descriptor to set */
			if (connfd > maxfd)
				maxfd = connfd;			/* for select */
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			//就绪描述符数目减1，若其值变为0，就可以避免进入下一个for循环，这样做让我们可以使用select的返回值来避免检查未就绪的描述符
			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		for (i = 0; i <= maxi; i++) 
		{	/* check all clients for data */
			//对于每个现有的客户连接
			if ( (sockfd = client[i]) < 0)
				continue;
			//我们要测试其描述符是否在select返回的描述符集中
			if (FD_ISSET(sockfd, &rset)) 
			{
				/*/如果是就从该客户读入一行文本并回射给它
				/存在问题：如果有恶意的客户连接到服务器，发送一个字节的数据（不是换行符）后进入睡眠（Dos攻击）
				*/
				if ( (n = Read(sockfd, buf, MAXLINE)) == 0) 
				{
						/*4connection closed by client */
					//如果该客户关闭了连接，那么就更新相应的数据结构
					Close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				} 
				else
					Writen(sockfd, buf, n);

				if (--nready <= 0)
					break;				/* no more readable descriptors */
			}
		}
	}
}
/* end fig02 */
