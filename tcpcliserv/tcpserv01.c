#include	"unp.h"

int main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	/*本地端口（服务器的众所周知的端口）由bind指定。
	bind调用中服务器指定的本地IP地址通常时通配IP地址。
	如果服务器在一个多宿主机上绑定通配IP地址，那么它可以在连接建立后通过调用getsockname来确定本地IP地址
	*/
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) 
	{
		clilen = sizeof(cliaddr);
		/*两个外地（客户）值由accept调用返回给服务器。如果另外一个程序由调用accept的服务器通过调用exec来执行，那么这个新程序可以在必要时调用getpeername来确定客户的IP地址和端口号。
		*/
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);

		if ( (childpid = Fork()) == 0) 
		{	/* child process */
			Close(listenfd);	/* close listening socket */
			//lib/str_echo.c
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}
