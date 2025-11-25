#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void* print_text(void* msg){

        printf("%s\n",(char*)msg);
        size_t length  = strlen(msg);
        size_t* plength = malloc(sizeof(size_t));
	 if (plength == NULL) {
	        fprintf(stderr, "Memory allocation failed\n");
	        return NULL;
	    }
	*plength = length;
        return plength;
}

int main(){

	pthread_t thread;
	const char* msg = "HELLo";
	if(pthread_create(&thread,NULL,print_text,(void*)msg)){
	  	fprintf(stderr,"creating fail\n");
		return 1;
	}

	size_t* length;
	if(pthread_join(thread,(void**)&length)){
		fprintf(stderr,"join fail\n");
		return 1;
	}

	printf("len = %zu\n",*length);
	free(length);
	return 0;
}
