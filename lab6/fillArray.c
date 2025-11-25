#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void * threadFunc(void *arg){

    size_t size = 10;
    int *arr = (int*)arg;
    //pthread_mutex_lock(&mutex);
	printf("\n\t second thread out = ");
    for(int i = 0;i<size;i+=2){
        *(arr+i) = rand()%size + 1;
        printf("%d ",*(arr+i));
    }
    printf("\n");
   // pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc,char *argv[]){

    int data[10];
    printf("Main thread started\n");
    pthread_t t1;
    int s = pthread_create(&t1,NULL,threadFunc,&data);
    if(s != 0){
       fprintf(stderr,"pthread_create error");
	exit(EXIT_FAILURE);
    }

    printf("Second thread started\n");
    printf("\n\t Main thread out = ");
    for(int i = 1;i<10;i+=2){
        data[i] = rand()%10 + 1;
        printf("%d ",data[i]);
    }
    printf("\n");

    pthread_join(t1, NULL);
 //   pthread_mutex_destroy(&mutex);
    printf("\n\t all array = ");
    for(int i = 0;i<10;i++){
        printf("%d ",data[i]);
    }
    printf("\n");

    exit(EXIT_SUCCESS);
}
