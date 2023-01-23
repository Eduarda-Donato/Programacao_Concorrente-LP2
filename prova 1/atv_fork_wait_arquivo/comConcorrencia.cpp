#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <string>

#define NUMBER_OF_FILES 6
#define STRING_SIZE 9

FILE *arq;
using namespace std;

int sizeOfArray(int arr[], int tam){
    int i{};
    for (i = 0; i < tam; i++){
        if (arr[i] == 0){
            break;
        }
    }

    return i;
}

int main(int argc, char *argv[]){

    char *arq_string;
    string user_string{};

    arq_string = (char*)malloc(sizeof(char)*STRING_SIZE);

    cout << "Digite a palavra que você quer que seja pesquisada nos arquivos" << endl;
    // scanf("%[\n]s", user_string);
    getline(cin, user_string);
    cout << user_string << endl;

    int pid_original{getpid()};
    int ret_fork;

    int *clones_pids;
    clones_pids = (int*)malloc(sizeof(int)*NUMBER_OF_FILES - 1);

    for(int i = 0; i < NUMBER_OF_FILES - 1; i++){
        if (getpid() == pid_original){
            ret_fork = fork();
            if(getpid() == pid_original){
                clones_pids[i] = ret_fork;
            }
            else{
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
                }

                if(arq == NULL){
                    cout << "Error" << endl;
                    return -1;
                }
            }
        }

        
    }

    int contador_string{0};
    int temp{0};
    bool read{};

    if (getpid() == pid_original){
        arq = fopen("../arquivos_texto/5.txt", "rt");

        if (arq == NULL){
            cout << "Error" << endl;
            return -1;
        }

        for (int i = 0; i < NUMBER_OF_FILES - 1; i++){
            // waitpid(clones_pids[i], &temp, 0);
            // cout << "Temp = " << temp << endl;
            wait(&temp);
            contador_string += temp;
        }

        cout << "Pid_original Contador_string antes = " << contador_string << endl;

        while(!feof(arq)){
            read = fgets(arq_string, STRING_SIZE , arq); //Read recebe true se veio uma string e falso se não
            if (read){
                if (user_string == arq_string){
                    cout << "Teste == pid_original" << endl;
                    contador_string++;
                }
                // cout << "arq_string = " << arq_string << endl << "user_string = " << user_string << endl;
            }
        }

        cout << "Pid_original Contador_string depois = " << contador_string << endl;
        // cout << contador_string << endl;
    }
    else{
        if (arq == NULL){
            cout << "Error" << endl;
            return -1;
        }

        while(!feof(arq)){
            read = fgets(arq_string, STRING_SIZE , arq); //Read recebe true se veio uma string e falso se não
            if (read){
                if (user_string == arq_string){
                    // cout << "Teste igual else" << endl;
                    contador_string++;
                }
            }
        }

        cout << "Contador_string = " << contador_string << endl;

        exit(contador_string);
    }

    if (getpid() == pid_original){
        cout << "A palavra " << user_string << " aparece " << contador_string << " vezes nos arquivos" << endl;
    }
    

    // cout << "sizeOfArray = " << sizeOfArray(clones_pids, NUMBER_OF_FILES - 1) << endl;


    return 0;
}