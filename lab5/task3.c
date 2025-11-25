#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdlib.h>
int main(){

	printf("PID = %d\n",getpid());
	printf("5s sleep");

	void *ptr = mmap(NULL,4096,PROT_READ | PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);

	printf("\nmemory with %p\n",ptr);
	printf("stop process(Ctrl+Z).\nUse 'fg' or 'kill -CONT %d' for continue.\n",getpid());
	kill(getpid(),SIGTSTP);
	char buf[32];
	printf("\nproc addr\n");
	snprintf(buf,sizeof(buf),"cat /proc/%d/maps",getpid());
	system(buf);
	sleep(2);
	printf("kill -9 %d\n");
	printf("done <3\n");
	snprintf(buf,sizeof(buf),"kill -9 %d",getpid());
	system(buf);
	return 0;
}
