#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define PORTA_SERVIDOR 9876
#define CHAVE "DiJqWHqKtiDgZySAv7ZX"

void * servidor(){
    
    int porta, nova_porta, tamanho, numbytes;
    struct sockaddr_in endereco_meu;
    struct sockaddr_in endereco_cliente;
    char buffer[255];
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_cliente = (char*) malloc(20*sizeof(char));
    
    strcpy(ip_meu, get_my_ip());
    
    /* cria socket. PF_INET define IPv4, SOCK_STREAM define TCP */
    porta = socket(PF_INET, SOCK_STREAM, 0);
    
    /* verifica se foi criado */
    if (porta == -1){
        perror("\n ::::: Erro: porta nao foi criada corretamente");
        exit(1);
    }
    
    /* porta criada, agora faz o bind com o numero da porta desejado */
    endereco_meu.sin_family = AF_INET;
    endereco_meu.sin_port = htons(PORTA_SERVIDOR);
    endereco_meu.sin_addr.s_addr = INADDR_ANY;
    memset(&(endereco_meu.sin_zero), '\0', 8);
    
    if (bind(porta,(struct sockaddr *) &endereco_meu, sizeof(struct sockaddr_in)) == -1){
        perror("\n ::::: Erro: servidor nao conseguiu fazer bind.\n");
    }
    
    /* agora faz uma chamada ao listen*/
    if (listen(porta,50)==-1){
        perror("\n ::::: Erro: servidor tem problemas com o listen\n");
        exit(1);
    }
    
    //printf("\n Servidor Peer on-line, aguardando conexões.\n");
    
    /* agora verifica se ha conexoes */
    while (1){
        tamanho = sizeof(struct sockaddr_in);
        
        /*Fica esperando aqui*/
        nova_porta = accept(porta, (struct sockaddr*)&endereco_cliente, &tamanho);
        
        strcpy(ip_cliente, inet_ntoa(endereco_cliente.sin_addr));
        //printf("\nip_cliente: %s", ip_cliente);
        //printf("\nip_meu: %s", ip_meu);
        
        if (nova_porta==-1){
            perror("\n ::::: Erro: servidor: accept retornou erro\n");
            //exit(1);
            break;
        }
        
        if ((numbytes=recv(nova_porta, buffer, 254, 0)) == -1) {
            perror("\n ::::: Erro: servidor: recv no cliente.\n");
            //exit(1);
            break;
        }
        buffer[numbytes] = '\0';
        printf("\n ::::: Servidor recebeu: %s", buffer);
        
        /*Agora testa se foi um ping que foi recebido.*/
        printf("\n ::::: ping pra comparar: %s", ping(ip_cliente, ip_meu));
        if(!strcmp(buffer, ping(ip_cliente, ip_meu))){
            printf("\n ::::: Servidor enviando pong...");
            
            if (fork()==0){ /* se for o filho */
                close(porta); /* o filho nao aceita conexoes a mais */
                
                if (send(nova_porta, pong(ip_meu, ip_cliente), 200, 0) == -1){
                    perror("\n ::::: Erro: servidor nao conseguiu mandar mensagem");
                }
                
                close(nova_porta);
                exit(0); /* tao logo termine, o filho pode sair */
            }
        }
        else{
            send(nova_porta, "Erro no seu ping.", 200, 0);
        }
        //close(nova_porta); /* essa parte somente o pai executa */
    }
}
