//pc tem recuros limitados, pedir mais processamento e memoria precisa ser tratado com cautela
//inconsistencia de memoria - armazena conjunto de informacoes que vai atrapalhar o andamento do seu programa
//EVITAR INCONSISTENCIA DE MEMORIA

/*
1
//problemas
//seção crítica - trava e semaforo
//sincronismo condicional - barreiras e semaforos

//secao critica









//ação atomica
//sem a instrução atomica ao escalonar uma thread pode pegar um valor antigo da região de memoria compatilhada
//fech_and_  ==  x++; recebe e incrementa

//para outras thrads perceberem oq ocorreu na seção critica e quem processou a seção critica, usa protocolos de entrada e saida, alem disso esses protocolos garatem que apenas uma thread por vez entre na seção critica
//protocolo de saida -conjunto de codigo que permite que uma thread entre caso esteja disponivel e se caso ela n esteja disponivel ele deve bloquear a outras
//espera ocupada (custoso) - while que segura a thread, a thread vai consumir processamento pois vai testar constantemente 
//protocolo de entrada - notifica as outra que a sc esta disponivel, a t anterior ja concluiu a sc e outra t deve ser convocada

//propriedades da solução do problema da sc

//exclusao mutua - t se excluam mutualmente garantido que apenas uma execute a sc num determinado instante
//ausencia de impasse(deadlock) - t bloqueia o acesso a um recurso que so sera desbloqueado se outra t liberar o recurso (uma thread ficara eternamente bloqueada esperando a outra)
// nao possua atraso desnecessaria, ela so realize acoes necessarias (protocolo de entrada, sc, ). se n tem ninguem na sc vc n pode retardar/impedido de entradar
// entrada evental (starvation - inanição) - se vc tem varias t tentando acessar a sc, todas elas vao entrar, eventualmente


//nem toda solução antende a 4 (spin lock n antende)
*/

/*
2
estados que a thread passa durante sua existencia:
cria o contexto do processo: todo processo tem pelo menos uma thread que sera escalonada (executa oq tiver no seu contexto)

//CICLO DE VIDA = DIAGRAMA DE ESTADOS
PRONTO -> (ESCALONADOR) -> EM EXECUÇÃO 
PRONTO <- (ESCALONADOR) <- EM EXECUÇÃO 
TEM UM CONJUNTO DE INSTRUÇÕES Q A T PRECISA PEGAR PARA CONTINUAR SUA EXECUÇÃO (n pode ficar nem no estado pronto nem no em execução)
EM EXECUÇÃO -> (E/S) -> ESTADO BLOQUEADO  (nao gasta processamento)
ESTADO BLOQUEADO -> PRONTO

//FUNÇÕES:
pthread_create (coloca na fila de pronto)
pthread_exit (finaliza a thread)
pthread_join (instruções de uma thread é trazida para ser processada em outra thread)

//FUNÇÕES SC:
pthread_mutex_t - tipo
pthread_mutex_lock - protocolo de entrada, antes da sc
pthread_mutex_unlock - protocolo de saida, depois da sc
*/

/*
3
obs: p seria uma struct COM ID, OERACAO , QUANTIDADE
algoritmo do ticket 

float saldo[QTD_CLIENTES];
int number =1, next =0
int turn[QTD_THREAD];

void* manipulaSaldo(void* p){
    //manipulação da struct;
    long id = data.indice;
    float op = data.opr;

    turn[id] = atomic_fetch_add(&number, 1); //recupera o valor da variavel e incrementa
    while(turn[id] != next);
    saldo[id] += op;
    atomic_fetch_add(&next, 1); //garante que ao chegar aqui já incrementa
    //next++ da certo, mas desperdiça processamento
FLOAT
}
*/

/*
4
barreira - vai testar em ciclos, as 6 threads vao gerar cada uma uma letra e depois vê se essa palavra é um palindromo ou n
ideia - as thrads geram a letra guarda no array e espera na barreira. O processamento vai ser em cima dos 6 caractere
*/

/*

*/