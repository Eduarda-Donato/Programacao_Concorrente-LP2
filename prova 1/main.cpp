/*
    Faça dois programas em C que solicitem uma string para o usuário (com até 8 caracteres).
    Os dois programas irão contar as ocorrências da string em um dos 6 arquivos anexados 
    à esta lista utilizando concorrência - um deles usará  uma função que busque nos arquivos 
    em sequencia e o outro que utilize processos  (um para cada arquivo). 
    No fim os dois programas deverão apresentar a soma das  ocorrências calculadas. 
    Comente o código com as diferença do uso de processos, do ponto de vista do programador 
    e do ponto de vista de desempenho/consumo de recursos.
*/

#include <iostream>
#include <string>
#include <string.h>

using namespace std;

FILE *arq;
const int STRING_SIZE{9};
const int NUMBER_OF_FILES{6};

int main (int argc, char *argv[]){

    cout << "Digite a String desejada a ser buscada nos arquivos de texto: ";
    int contador_string{0};
    string user_string{};
    getline(cin, user_string);

    bool read{};
    char * arq_string{}; //Não se pode usar arq_string como String, já que a função fgets
    //foi feita com string de C, e a conversão não é possíveL
    arq_string = (char*)malloc(sizeof(char)*(user_string.size() + 1));

    cout << "Palavra requerida: " << user_string << endl;
    // arq = fopen("arquivos_texto/0.txt", "rt");
    for (int i = 0; i < NUMBER_OF_FILES; i++){
        switch(i){
            case 0:{
                arq = fopen("../arquivos_texto/0.txt", "rt");
                break;
            }

            case 1:{
                arq = fopen("../arquivos_texto/1.txt", "rt");
                break;
            }

            case 2:{
                arq = fopen("../arquivos_texto/2.txt", "rt");
                break;
            }

            case 3:{
                arq = fopen("../arquivos_texto/3.txt", "rt");
                break;
            }

            case 4:{
                arq = fopen("../arquivos_texto/4.txt", "rt");
                break;
            }

            case 5:{
                arq = fopen("../arquivos_texto/5.txt", "rt");
                break;
            }
        }

        if (arq == NULL){
            cout << "Arquivo errado" << endl;
            break;
        }

            //A função feof verifica se o arquivo finalizou. Então !feof(arq) verifica
//se ainda existem linhas a serem lidas no arquivo
        while(!feof(arq)){ 
            read = fgets(arq_string, STRING_SIZE , arq); //Read recebe true se veio uma string e falso se não
            if (read){
                if (arq_string == user_string){
                    contador_string++;
                }
            }
        }
    }

    // cout << "Antes do while" << endl;


    cout << "A string " + user_string + " aparece " <<  contador_string << " vezes nos arquivos .txt" << endl;

    return 0;
}