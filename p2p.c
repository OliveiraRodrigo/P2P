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
    
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\n== P2P =====================================\n");
    printf("\n __________________________________________\n");
    printf("\n Bem-vindo ao programa P2P!\n");
    
    pthread_t thread_servidor;
    pthread_t thread_cliente;
    
    pthread_create(&thread_servidor, NULL, servidor, NULL/*(void*)&args*/);
    //pthread_create(&thread_cliente, NULL, cliente, NULL/*(void*)&args*/);
    
    //servidor();
    cliente();
    
    
    
    
    
    return (EXIT_SUCCESS);
}

