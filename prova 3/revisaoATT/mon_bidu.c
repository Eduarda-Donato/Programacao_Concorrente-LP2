//mutex e monitor
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define QTD_THREADS 10

typedef struct {
    int operacao;
    char codigo;
    float valor;
}op_bancaria;

typedef struct {
    char* filename;
    FILE* filedescript;
    pthread_mutex_t mutex;
}monitor;

monitor mon;

void* filewrite(void* p){
    op_bancaria *ob = (op_bancaria*)p;
    
    /*printf("ob.operacao = %d\n", ob->operacao);
    printf("ob.codigo = %c\n", ob->codigo);
    printf("ob.valor = %f\n", ob->valor);*/

    pthread_mutex_lock(&mon.mutex); //mesmo mutex para a leitura
    fwrite(&ob, sizeof(op_bancaria), 1, mon.filedescript); //1 = qtd de vez q quer escrevevr
    pthread_mutex_unlock(&mon.mutex);
}

void* fileread(void* p){
    op_bancaria *ob = (op_bancaria*)p;

    pthread_mutex_lock(&mon.mutex);
    fread(&ob, sizeof(op_bancaria), 1, mon.filedescript);
    printf("ob.operacao = %d\n", ob->operacao);
    printf("ob.codigo = %c\n", ob->codigo);
    printf("ob.valor = %f\n", ob->valor);
    pthread_mutex_unlock(&mon.mutex);
}



int main(int argc, char const *argv[]){
    pthread_t ths[QTD_THREADS];
    op_bancaria obs[QTD_THREADS];

    time_t t;
    srand(time(&t));

    mon.filename = (char*) malloc(sizeof(char)*20);
    strcpy(mon.filename, "arquivo.bin");
    mon.filedescript = fopen(mon.filename, "w+b");
    pthread_mutex_init(&mon.mutex, NULL);

    for (size_t i = 0; i < QTD_THREADS; i++){
        obs[i].operacao = rand() % 5;
        obs[i].codigo = 65 + (rand() % 5);
        obs[i].valor = rand() % 1000;
    }

    for (size_t i = 0; i < QTD_THREADS; i++){
        pthread_create(&ths[i], NULL, filewrite, (void*)&obs[i]);
        pthread_create(&ths[i], NULL, fileread, (void*)&obs[i]);
    }

    for (size_t i = 0; i < QTD_THREADS; i++){
        pthread_join(ths[i], NULL);
    }
    
    fclose(mon.filedescript);

    return 0;
}

