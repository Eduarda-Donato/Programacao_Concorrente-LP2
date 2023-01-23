/*
    Faça dois programas em C que solicitem uma string para o usuário (com até 8 caracteres).
    Os dois programas irão contar as ocorrências da string em um dos 6 arquivos anexados 
    à esta lista utilizando concorrência - um deles usará  uma função que busque nos arquivos 
    em sequencia e o outro que utilize processos  (um para cada arquivo). 
    No fim os dois programas deverão apresentar a soma das  ocorrências calculadas. 
    Comente o código com as diferença do uso de processos, do ponto de vista do programador 
    e do ponto de vista de desempenho/consumo de recursos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

FILE *arquivo;

int main(/*int arc, char* argv[]*/void){
    int quant_string;
    char user_string[9];
    char arq_string[9];
    bool read;
    //arq_string = (char*)malloc(sizeof(char)*(user_string.size() + 1));

    printf("DIgite a palavra desejada: ");
    scanf("%s", user_string);

    for(int i = 0; i <= 5; i++){
        switch (i){
        case 0:
            arquivo = fopen("0.txt", "rt");
            break;
        case 1:
            arquivo = fopen("1.txt", "rt");
            break;
        case 2:
            arquivo = fopen("2.txt", "rt");
            break;
        case 3:
            arquivo = fopen("3.txt", "rt");
            break;
        case 4:
            arquivo = fopen("4.txt", "rt");
            break;
        case 5:{
            arquivo = fopen("5.txt", "rt");
            break;
            }
        default:
            break;
        }
        if (arquivo == NULL){
            printf("Erro na abertura do arquivo");
        }
        while (!feof(arquivo)){
        read = fgets(arq_string, 9,arquivo);
        if(read){
            if((strcmp(arq_string,user_string)) == 0){
                quant_string++;
            }
        }
    }
    }
    
    printf("A string %s aparece %d vezes nos arquivos .txt", user_string,quant_string);
    
    return 0;
}
