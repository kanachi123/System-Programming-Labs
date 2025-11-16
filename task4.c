#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/mman.h>
#include <stdlib.h>

void sigbus_handler(int sig,siginfo_t *si,void *unused){

	printf("\t\t\t\t\t!!! Caught SIGBUS !!!\t\t\t\t\t\t\n");
	printf("\t\t\t\t\t!!! Fault address: %p !!!\t\t\t\t\t\t\n",si->si_addr);

	switch(si->si_code)
	{	case BUS_ADRALN:
			printf("Misaligned addres\n"); break;
		case BUS_ADRERR:
			printf("Invalid physical address\n"); break;
		case BUS_OBJERR:
			printf("Device error / mmap error"); break;
		default:
			printf("Unknown\n");
	}

	exit(1);

}

int main(){

	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = sigbus_handler;
	sigaction(SIGBUS,&sa,NULL);

	printf("PID = %d\n",getpid());
	printf("5s sleep");

	char *ptr = mmap(NULL,4096,PROT_READ | PROT_WRITE,MAP_ANONYMOUS | MAP_PRIVATE,-1,0);

	*(ptr + 4099) = '~';

	printf("\nmemory with %p\n",ptr);
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
