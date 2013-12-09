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
    
    int porta = servidor();
    
    printf("\n\n\n\n\n\n\n\n\n\n");
    printf("\n== P2P =====================================\n");
    printf("\n __________________________________________\n");
    printf("\n Bem-vindo ao programa P2P!\n");
    
    pthread_create(&thread_servidor, NULL, start_connection, (void*) porta);
    //servidor();
    
    pthread_create(&thread_cliente, NULL, cliente, NULL);
    //cliente();
    
    //pthread_join(thread_servidor, NULL);
    pthread_join(thread_cliente, NULL);
    
    return (EXIT_SUCCESS);
}

