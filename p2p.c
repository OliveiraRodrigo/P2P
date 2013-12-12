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

int main(int argc, char** argv) {
    
    pthread_t thread_servidor;
    pthread_t thread_cliente;
    int fim_servidor, fim_cliente;
    
    intptr_t porta = servidor();
    
    printf("\033[2J");//limpa a tela
    printf("\033[46m");
    black
    printf("\n   P2P                                                                \n");
    printf("\033[40m");
    cyan
    printf("\n Bem-vindo ao programa P2P!\n");
    white
    printf("\n Para exibir os comandos digite ");orange printf("help");white printf(".\n");
    cyan
    printf("\n______________________________________________________________________\n");
    white
    
    pthread_create(&thread_servidor, NULL, start_connection, (void*) porta);
    //servidor();
    
    pthread_create(&thread_cliente, NULL, cliente, NULL);
    //cliente();
    
    //pthread_join(thread_servidor, NULL);
    pthread_join(thread_cliente, NULL);
    
    return (EXIT_SUCCESS);
}

