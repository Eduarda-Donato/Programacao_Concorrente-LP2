#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

#define NTHREADS 5

int lock = 0;
float global = 0;



void* cs (void*p){

    while (__atomic_test_and_set(&lock, 0)); 
    global += 10;
    lock = 0;    

}

int main(){

printf("\nvalor de global antes das threads: %.0f\n\n", global);

pthread_t ths[NTHREADS];

for (long i = 0; i < NTHREADS; i++)
{
pthread_create(&ths[i], 0, cs , (void*)i);
}
for (long i = 0; i < NTHREADS; i++)
{
pthread_join(ths[i], NULL);
}

printf("\nvalor de global após as threads: %.0f\n\n", global);

    return 0;
}