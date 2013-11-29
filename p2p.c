#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define PORTA_SERVIDOR 9876

int main(/*int argc, char** argv*/) {
    
    char** parametros;
    int i, j, quit;
    
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\n== P2P =====================================\n");
    printf("\n __________________________________________\n");
    printf("\n Bem-vindo ao programa P2P!\n");
    
    pthread_t thread_servidor;
    pthread_t thread_cliente;
    
    printf("\n\n P2P:> ");
    
    pthread_create(&thread_servidor, NULL, servidor, NULL/*(void*)&args*/);
    //pthread_create(&thread_cliente, NULL, cliente, NULL/*(void*)&args*/);
    
    //servidor();
    
    /* Aloca espaco para 4 parametros com 50 caracteres cada */
    for(i = 0; i < 4; i++){
        parametros[i] = (char*) malloc(50*sizeof(char));
    }
    
    i = 0;
    j = 0;
    parametros[i][j] = getchar();
    j++;
    
    while(parametros[i][j-1] != '\n'){
        parametros[i][j] = getchar();
        if(parametros[i][j] == ' '){
            parametros[i][j] = '\0'; // Descarta o ' '.
            j = 0;
            i++;
        }
        else{
            j++;
        }
    }
    parametros[i][j-1] = '\0'; // Descarta o '\n'.
    
    /* Comandos do cliente - Exemplos:
     * 
     * try 123.321.1.2              (Manda um ping pra este ip e recebe um pong.)
     * 
     * login minhasenha 123.321.1.2 (Tenta autenticar-se com este ip. Se receber
     *                               authenticate-back codigo 200, envia agent-list,
     *                               recebe agent-list-back, insere na lista os
     *                               ips recebidos e retorna a lista pro usuario.)
     * 
     * list 159.4.4.4               (Envia archive-list para este ip. Se receber
     *                               archive-list-back, retorna a lista pro usuario.
     *                               Deve antes estar logado com este ip.)
     * 
     * down 2 159.4.4.4             (Envia arquive-request a 555.5.5.5 pedindo o arquivo de id:"2".)
     * 
     * quit                         (Envia um end-connection a todos os ips da lista.)
     * 
     */
    
    cliente(parametros);
    
    return (EXIT_SUCCESS);
}

