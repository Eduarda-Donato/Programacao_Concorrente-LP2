#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <time.h>
#include <errno.h>


int main(void) {
	char* my_args[3] = {"./pa-test", "STRING_TEST", NULL};
	printf("Sou o programa execv\n");
	long x = 0;
	while (x < 9999999999) {
		x++;
	}
	int ret_fork = fork();
	if (ret_fork == 0) {//processo clonado
		x = 0;
		while (x < 9999999999) x++;
		execv("./pa-test", my_args);
	}else{
        printf("Sou o processo original\n");
	    printf("Sou o programa exec-test\n");
        x = 0;

	    while (x < 9999999999) { x++; }
    }
	

	
	return 0;

}
