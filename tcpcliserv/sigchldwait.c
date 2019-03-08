#include	"unp.h"

void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	/*父进程一旦调用了wait就立即阻塞自己，由wait自动分析是否当前进程的某个子进程已经退出，
	如果让它找到了这样一个已经变成僵尸的子进程，wait就会收集这个子进程的信息，并把它彻底销毁后返回；
	如果没有找到这样一个子进程，wait就会一直阻塞在这里，直到有一个出现为止。
	不能循环使用wait，因为没有办法防止wait在正运行的子进程尚有未终止时阻塞
	*/
	pid = wait(&stat);
	printf("wait version ===> child %d terminated\n", pid);
	return;
}
