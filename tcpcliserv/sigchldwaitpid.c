#include	"unp.h"

void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	/*/第一个参数：> 0时，只等待进程ID等于pid的子进程，不管其它已经有多少子进程运行结束退出了，只要指定的子进程还没有结束，waitpid就会一直等下去。
	= -1时，等待任何一个子进程退出，没有任何限制，此时waitpid和wait的作用是一模一样的
	= 0时，等待同一个进程组中的任何子进程，如果子进程已经加入了别的进程组，waitpid不会对它做任何理睬
	< -1时，等待一个指定进程组中的任何子进程，这个进程组的ID等于pid的绝对值
	/第三个参数提供了一些额外的选项来控制waitpid，如果使用WNOHANG参数即使没有子进程退出它也会立即返回，不会像wait那样永远等下去。
	*/
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("waitpid version ===> child %d terminated\n", pid);
	return;
}
