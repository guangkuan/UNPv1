#include	"unp.h"

int main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	/*C语言的优先规则（小于运算符的优先级高于赋值运算符）
	左括号间的空格表示比较运算的左侧同时也是一个赋值运算
	*/
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	//bzero把整个结构清零
	bzero(&servaddr, sizeof(servaddr));
	//置地址族为AF_INET
	servaddr.sin_family = AF_INET;
	/*外地（服务器）端口号为13（这是时间获取服务器的众所周知端口，支持该服务的任何TCP/IP主机都使用这个端口号）
	htons是将整型变量从主机字节顺序转变成网络字节顺序， 就是整数在地址空间存储方式变为高位字节存放在内存的低地址处。
	*/
	servaddr.sin_port   = htons(13);	/* daytime server */
	//inet_aton是一个计算机函数，功能是将一个字符串IP地址转换为一个32位的网络序列IP地址。
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	/*第二个参数指向的套接字地址结构指定的服务器建立一个TCP连接。该套接字地址结构的长度也必须作为该函数的第三个参数指定
	每当一个套接字函数需要一个指向某个套接字地址结构的指针时，这个指针必须强制类型转换成一个指向通用套接字地址结构的指针
	本地（客户）IP地址和端口号通常由内核作为connect的一部分来选定。
	*/
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	/*如果数据量很大，不能确保一次read调用能返回服务器的整个应答。因此从TCP套接字读取数据时，总是需要把read编写在某个循环中
	当read返回0（表明对端关闭连接）或负值（表明发生错误）时终止循环
	*/
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) 
	{
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}
