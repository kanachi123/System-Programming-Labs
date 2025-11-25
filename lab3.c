#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* print_text(void* arg){

	for(int i = 1;i<6;i++){
		printf("~ %d\n",i);
		sleep(1);
	}
	printf("second thread end\n");
	return NULL;
}

int main(){

	pthread_t thread;

	if(pthread_create(&thread,NULL,print_text,NULL)){

		fprintf(stderr,"creating fail\n");
		return 1;
	}

	if(pthread_detach(thread)){
		fprintf(stderr,"detaching fail\n");
	}
	printf("sleep in main thread\n");

	for(int i = 1;i<10;i++){
                printf("%d\n",i);
                sleep(1);
        }
	printf("main thread end\n");

	return 0;
}
