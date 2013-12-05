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
    int ping, logado, saiu;
    struct sockaddr_in endereco_meu;
    struct sockaddr_in endereco_cliente;
    char buffer[1000];
    char * ips_string = (char*) malloc(1000*sizeof(char));
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_cliente = (char*) malloc(20*sizeof(char));
    protocolo protoin;
    archive_def * files;
    
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
    
/* Aguarda conexoes ***********************************************************/
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
        
/* Espera ping, envia pong ****************************************************/
        ping = 0;
        while(!ping){
            printf("\nservidor entrou no ping.");
            if ((numbytes = recv(nova_porta, buffer, 999, 0)) == -1) {
                perror("\n ::::: Erro: Servidor nao conseguiu receber 'ping'.\n");
                break;
            }
            buffer[numbytes] = '\0';
            //printf("\n ::::: Servidor recebeu: %s\n", buffer);
            
            //Testa se recebeu um ping ok.
            protoin = set_proto(buffer);
            if(protoin.ok && !strcmp(protoin.command, "ping")){
                ping = 1;
                //printf("\n ::::: Servidor enviando pong...\n");
                if (send(nova_porta, pong(ip_meu, ip_cliente), 200, 0) == -1){
                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'pong'.");
                }
            }
            else{
                send(nova_porta, "Erro: eu esperava um 'ping'.", 200, 0);
            }
        }
        printf("\nservidor saiu do ping.");
/* Espera authenticate, envia authenticate-back *******************************/
/* Tambem aceita ping, devolve pong *******************************************/
        logado = 0;
        while(!logado){
            printf("\nservidor entrou no login.");
            if((numbytes = recv(nova_porta, buffer, 999, 0)) == -1) {
                perror("\n ::::: Erro: Servidor nao conseguiu receber.\n");
                break;
            }
            buffer[numbytes] = '\0';
            //printf("\n ::::: Servidor recebeu: %s\n", buffer);
            
            //Testa se recebeu um authenticate ok.
            protoin = set_proto(buffer);
            if(protoin.ok){
                if(!strcmp(protoin.command, "authenticate")){
                    if(!strcmp(protoin.passport, CHAVE)){
                        logado = 1;
                        //printf("\n ::::: Servidor enviando authenticate-back...\n");
                        if(send(nova_porta, authenticate_back(200, ip_meu, ip_cliente), 200, 0) == -1){
                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'authenticate-back'.");
                        }
                    }
                    else{
                        send(nova_porta, authenticate_back(203, ip_meu, ip_cliente), 200, 0);
                    }
                }
                else{
                    if(!strcmp(protoin.command, "ping")){
                        printf("\nservidor vai enviar pong.");
                        send(nova_porta, pong(ip_meu, ip_cliente), 200, 0);
                    }
                    else{
                        send(nova_porta, "Erro 401: protocolo inesperado", 200, 0);
                        //O correto seria enviar com codigo 401 o mesmo comando recebido.
                    }
                }
            }
            else{
                send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 200, 0);
            }
        }
        printf("\nservidor saiu do login.");
/* Espera qualquer comando ****************************************************/
        saiu = 0;
        while(!saiu){
            printf("\nservidor entrou no qualquer comando.");
            if((numbytes = recv(nova_porta, buffer, 999, 0)) == -1) {
                perror("\n ::::: Erro: Servidor nao conseguiu receber.\n");
                break;
            }
            buffer[numbytes] = '\0';
            //printf("\n ::::: Servidor recebeu: %s\n", buffer);
            
            //Recebeu o que?
            protoin = set_proto(buffer);
            if(protoin.ok){
                if(!strcmp(protoin.command, "authenticate")){
                    //Ja esta autenticado, mas tudo bem, manda de novo um auth-back.
                    send(nova_porta, authenticate_back(200, ip_meu, ip_cliente), 200, 0);
                }
                else{
                    if(!strcmp(protoin.command, "agent-list")){
                        if(send(nova_porta, agent_list_back(200, ips_string, ip_meu, ip_cliente), 200, 0) == -1){
                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-list-back'.");
                        }
                    }
                    else{
                        if(!strcmp(protoin.command, "archive-list")){
                            if(send(nova_porta, archive_list_back(200, files, ip_meu, ip_cliente), 200, 0) == -1){
                                perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-list-back'.");
                            }
                        }
                        else{
                            if(!strcmp(protoin.command, "archive-request")){
                                if(1/*tem_file(protoin.id)*/){
                                    if(send(nova_porta, archive_request_back(302, files[0], ip_meu, ip_cliente), 200, 0) == -1){
                                        perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-request-back'.");
                                    }
                                }
                                else{
                                    send(nova_porta, archive_request_back(404, files[0], ip_meu, ip_cliente), 200, 0);
                                }
                            }
                            else{
                                if(!strcmp(protoin.command, "end-conection")){
                                    saiu = 1;
                                    remove_ip(ips_string, ip_cliente);
                                    close(nova_porta); //Serah?
                                }
                                else{
                                    if(!strcmp(protoin.command, "ping")){
                                        send(nova_porta, pong(ip_meu, ip_cliente), 200, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else{
                send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 200, 0);
            }
        }
        if (fork()==0){ // se for o filho
            //printf("\nxxxxx fork == 0 xxxxx\n");
            close(porta); /* o filho nao aceita conexoes a mais */
            
            /*if (send(nova_porta, pong(ip_meu, ip_cliente), 200, 0) == -1){
                perror("\n ::::: Erro: servidor nao conseguiu mandar mensagem");
            }*/
            
            close(nova_porta);
            exit(0); // tao logo termine, o filho pode sair
        }
        else{
            //printf("\nxxxxx fork != 0 xxxxx\n");
        }
        //close(nova_porta); /* essa parte somente o pai executa */
    }
}
