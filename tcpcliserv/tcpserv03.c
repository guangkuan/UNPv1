#include	"unp.h"

int main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	void				sig_chld(int);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, sig_chld);

	for ( ; ; ) 
	{
		clilen = sizeof(cliaddr);
		/*SIGCHLD信号是在父进程阻塞于慢系统调用(accept)时由父进程捕获的，内核就会使accept返回一个EINTR错误（被中断的系统调用）。
		而父进程不处理该错误，于是终止。通过设置SA_RESTART标志自动重启被中断的系统调用。
		当阻塞于某个慢系统调用的一个进程捕获某个信号且相应信号处理函数返回时，该系统调用可能返回一个EINTR错误，下面这段代码做的事情就是自己重启被中断的系统调用
		*/
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) 
		{
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				err_sys("accept error");
		}

		if ( (childpid = Fork()) == 0) 
		{	/* child process */
			Close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}
