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

int main(int argc, char** argv) {
    
    pthread_t thread_servidor;
    pthread_t thread_http_server;
    pthread_t thread_cliente;
    
    printf("\033]0;:: P2P :: %s\007", get_my_ip()); // Altera o titulo do terminal
    
    intptr_t porta_servidor = servidor(PORTA_SERVIDOR);
    intptr_t porta_http = servidor(PORTA_HTTP);
    
    clear_screen
    
    bg_cyan white bold printf("\n   P2P                                                                  ");
    defaults printf("\n\n "); cyan under printf("Bem-vindo ao programa P2P!"); reset printf("\n");
    defaults white printf("\n Para exibir os comandos digite "); orange printf("help"); white printf(".\n");
    cyan printf("\n________________________________________________________________________\n");
    
    pthread_create(&thread_servidor, NULL, start_connection, (void*) porta_servidor);
    
    pthread_create(&thread_http_server, NULL, httpReq, (void*) porta_http);
    
    pthread_create(&thread_cliente, NULL, cliente, NULL);
    
    pthread_join(thread_cliente, NULL);
    
    return (EXIT_SUCCESS);
}

