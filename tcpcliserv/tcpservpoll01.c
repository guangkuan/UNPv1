/* include fig01 */
#include	"unp.h"
#include	<limits.h>		/* for isom */

int main(int argc, char **argv)
{
	int					i, maxi, listenfd, connfd, sockfd;
	int					nready;
	ssize_t				n;
	char				buf[MAXLINE];
	socklen_t			clilen;
	/*/在select的版本，我们必须分配一个client数组以及一个名为rset的描述符集。
	/改用poll后，我们只需分配一个pollfd结构的数组来维护客户信息，而不必分配另外一个数组。
	*/
	int isom = sysconf(_SC_OPEN_MAX);
	struct pollfd		client[isom];
	struct sockaddr_in	cliaddr, servaddr;

	//isom为每个进程可以打开的最大文件数
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	//把client数组的第一项用于监听套接字，并把其余各项的描述符成员置为-1
	client[0].fd = listenfd;
	//给第一项设置POLLIN事件，这样当由新的连接准备好被接受时poll将通知我们
	client[0].events = POLLIN;
	for (i = 1; i < isom; i++)
		client[i].fd = -1;		/* -1 indicates available entry */
	//maxi为client数组当前正在使用的最大下标值
	maxi = 0;					/* max index into client[] array */
/* end fig01 */

/* include fig02 */
	for ( ; ; ) 
	{
		/*/poll提供的功能与select类似，不过在处理流设备时，它能够提供额外的信息
		/第一个参数是指向一个结构数组第一个元素的指针。每个数组元素都是一个pollfd结构，用于指定测试某个给定描述符fd的条件;
		要测试的条件由events成员指定，函数在相应的revents成员中返回该描述符的状态。（每个描述符都有两个变量，一个为调用值，另一个为返回结果，从而避免使用值-结果参数）
		/结构数组中元素的个数时由nfds（第二个）参数指定
		/timeout（第三个）参数指定poll函数返回前等待多久时间*/
		nready = Poll(client, maxi+1, INFTIM);

		//当一个新的连接被接受后
		if (client[0].revents & POLLIN) 
		{	/* new client connection */
			clilen = sizeof(cliaddr);
			connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
#ifdef	NOTDEF
			printf("new client: %s\n", Sock_ntop((SA *) &cliaddr, clilen));
#endif
			/*在client数组中查找第一个描述符成员为负的可用项。
			从下标1开始搜索，因为client[0]固定用于监听套接字。
			*/
			for (i = 1; i < isom; i++)
				if (client[i].fd < 0) 
				{
					client[i].fd = connfd;	/* save descriptor */
					break;
				}
			if (i == isom)
				err_quit("too many clients");

			client[i].events = POLLIN;
			if (i > maxi)
				maxi = i;				/* max index in client[] array */

			if (--nready <= 0)
				continue;				/* no more readable descriptors */
		}

		//检查已连接套接字可读信息
		for (i = 1; i <= maxi; i++) 
		{	/* check all clients for data */
			if ( (sockfd = client[i].fd) < 0)
				continue;
			if (client[i].revents & (POLLIN | POLLERR)) 
			{
				if ( (n = read(sockfd, buf, MAXLINE)) < 0) 
				{
					if (errno == ECONNRESET) 
					{
							/*4connection reset by client */
#ifdef	NOTDEF
						printf("client[%d] aborted connection\n", i);
#endif
						Close(sockfd);
						client[i].fd = -1;
					} 
					else
						err_sys("read error");
				} 
				else if (n == 0) 
				{
						/*4connection closed by client */
#ifdef	NOTDEF
					printf("client[%d] closed connection\n", i);
#endif
					Close(sockfd);
					client[i].fd = -1;
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
