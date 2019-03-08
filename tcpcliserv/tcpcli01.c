#include	"unp.h"

int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	//外地（服务）IP地址和外地端口必须在客户调用connect时指定，而本地（客户）IP地址和端口号通常由内核作为connect的一部分来选定。
	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	/*/当FIN到达套接字时，客户正阻塞在fgets调用上。
	客户实际上在应对两个描述符————套接字和用户输入，它不能单纯阻塞在这两个源中某个特定源输入上，而是应该阻塞在其中任何一个源输入上。
	这样导致的结果是，一旦杀死服务器子进程，客户不会立即被告知已收到的FIN。
	*/
	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
