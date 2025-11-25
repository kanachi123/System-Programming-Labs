#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void * threadFunc(void *arg){

    printf("New Thread Started\n");
    pthread_mutex_lock(&mutex);
    *((double*)arg) = pow(*((double*)arg),2);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc,char *argv[]){

    printf("Main thread started\n");
    pthread_t t1,t2;
    double val = 0;
    scanf("%lf",&val);
    printf("\n val = %g\n",val);
    int s = pthread_create(&t1,NULL,threadFunc,&val);
    if(s != 0){
       fprintf(stderr,"pthread_create error");
	exit(EXIT_FAILURE);
    }
    pthread_join(t1, NULL);
    printf("pow(val,2) = %g\n",val);
    s = pthread_create(&t2,NULL,threadFunc,&val);
    if(s != 0){
       fprintf(stderr,"pthread_create error");
	exit(EXIT_FAILURE);
    }
    pthread_join(t2, NULL);
    printf("pow(val,3) = %g\n",val);

    pthread_mutex_destroy(&mutex);

    exit(EXIT_SUCCESS);
}
