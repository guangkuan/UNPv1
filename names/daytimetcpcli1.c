#include	"unp.h"

int main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;
	struct in_addr		**pptr;
	struct in_addr		*inetaddrp[2];
	struct in_addr		inetaddr;
	struct hostent		*hp;
	struct servent		*sp;

	if (argc != 3)
		err_quit("usage: daytimetcpcli1 <hostname> <service>");

	//第一个命令行参数时主机名
	if ( (hp = gethostbyname(argv[1])) == NULL) 
	{
		//尝试使用inet_aton函数，确定其参数是否已时ASCII格式的地址，若时则构造一个由相应的地址构成的单元素列表。
		if (inet_aton(argv[1], &inetaddr) == 0) 
		{
			err_quit("hostname error for %s: %s", argv[1], hstrerror(h_errno));
		} 
		else 
		{
			inetaddrp[0] = &inetaddr;
			inetaddrp[1] = NULL;
			pptr = inetaddrp;
		}
	} 
	else 
	{
		pptr = (struct in_addr **) hp->h_addr_list;
	}

	//第二个命令行参数时服务名
	if ( (sp = getservbyname(argv[2], "tcp")) == NULL)
		err_quit("getservbyname error for %s", argv[2]);

	//该循环为服务器主机的每个地址执行一次，直到connect成功或IP地址列表试完为止。
	for ( ; *pptr != NULL; pptr++) 
	{
		sockfd = Socket(AF_INET, SOCK_STREAM, 0);

		bzero(&servaddr, sizeof(servaddr));
		//以服务器主机的IP地址和端口装填网际套接字地址结构
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = sp->s_port;
		memcpy(&servaddr.sin_addr, *pptr, sizeof(struct in_addr));
		printf("trying %s\n", Sock_ntop((SA *) &servaddr, sizeof(servaddr)));

		if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) == 0)
			break;		/* success */
		err_ret("connect error");
		close(sockfd);
	}
	if (*pptr == NULL)
		err_quit("unable to connect");

	while ( (n = Read(sockfd, recvline, MAXLINE)) > 0) 
	{
		recvline[n] = 0;	/* null terminate */
		Fputs(recvline, stdout);
	}
	exit(0);
}
