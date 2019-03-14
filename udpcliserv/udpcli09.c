#include	"unp.h"

int main(int argc, char **argv)
{
	int					sockfd;
	socklen_t			len;
	struct sockaddr_in	cliaddr, servaddr;

	if (argc != 2)
		err_quit("usage: udpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	len = sizeof(cliaddr);
	/*已连接UDP套接字还可用来确定用于某个特定目的地的外出接口。
	/在UDP套接字上调用connect并不给对端主机发送任何消息，它完全是一个本地操作，只是保存对端的IP地址和端口号。
	/在一个未绑定端口号的UDP套接字上调用connect同时也给该套接字指派一个临时端口。
	*/
	Getsockname(sockfd, (SA *) &cliaddr, &len);
	printf("local address %s\n", Sock_ntop((SA *) &cliaddr, len));

	exit(0);
}
