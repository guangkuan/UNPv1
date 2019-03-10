#include	"unp.h"

void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	/*/fd_set其实是一个数组的宏定义，实际上时一个long类型的数组，每一个数组元素都能与一打开的文件句柄（socket、文件、管道、设备等）
	建立联系，建立联系的工作由程序员完成，当调用select()时，由内核根据IO状态修改fd_set的内容，由此通知执行了select()的进程进程哪个句柄可读。
	/FD_ZERO(fd_set* fdset) 将指定的文件描述符集清空。
	在对文件描述符集合进行设置前，必须对其进行初始化，
	如果不清空，由于在系统分配内存空间后，通常并不作清空处理，所以结果是不可知的。
	/FD_SET(int fd, fd_set* fdset) 用于在文件描述符集合中增加一个新的文件描述符。
	/FD_CLR(int fd, fd_set* fdset) 用于在文件描述符集合中删除一个文件描述符。
	/FD_ISSET(int fd, fd_set* fdset) 用于测试指定的文件描述符是否在该集合中。
	*/
	FD_ZERO(&rset);
	for ( ; ; ) 
	{
		/*fileno函数把标准I/O文件指针转换为对应的描述符*/
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		/*/该函数允许进程指示内核等待多个事件中的任何一个发生，并只在有一个或多个事件发生或经历一段指定的事件后才唤醒它。
		/最后一个参数timeout，告知内核等待所指定描述符中的任何一个就绪可花多少时间，const限定词表示它在函数返回时不会被select修改。
		/中间的三个参数readset、writeset和exceptset指定我们要让内核测试读、写和异常条件的描述符。
		这三个参数都是值-结果参数。调用该函数时，我们指定所关心的描述符的值，该函数返回时，结果将指示哪些描述符已就绪。
		描述符集内任何与未就绪描述符对应的位返回时均清成0。
		/第一个参数maxfdp1指定待测试的描述符个数，它的值是待测试的最大描述符加1。
		*/
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		/*/在批量方式下，标准输入中的EOF并不意味着我们同时也完成了从套接字的读入；
		可能仍有请求在去往服务器的路上，或者仍由应答在返回客户的路上。
		/我们需要一种关闭TCP连接其中一半的方法。
		也就是说，我们想给服务器发送一个FIN，告诉它我们已经完成了数据发送，但是仍然保持套接字描述符打开以便读取。
		*/
		if (FD_ISSET(sockfd, &rset)) 
		{	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}

		if (FD_ISSET(fileno(fp), &rset)) 
		{  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
