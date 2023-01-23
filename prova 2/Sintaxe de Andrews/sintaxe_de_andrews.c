#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

int global = 0;

int in1 = 0;
int in2 = 0;

void* cs1(void*p){

 

while (in2 == 1);
in1 = 1;
global ++; 
sleep(2);

printf("\nvalor global cs1: %d\n", global);

in1 = 0;


}
void* cs2(void*p){


while (in1 == 1);
in2 = 1;
global ++; 
sleep(2);

printf("\nvalor global cs2: %d\n", global);


in2 = 0;

    

}
int main(void){


printf("\n[MAIN] Valor global antes das threads: %d\n", global);

pthread_t t1, t2;

pthread_create(&t1, 0, cs1, NULL);

pthread_create(&t2, 0, cs2, NULL);

pthread_join(t1, NULL);

pthread_join(t2, NULL);


printf("\n[MAIN] Valor global depois das threads: %d\n", global);

    return 0;
}