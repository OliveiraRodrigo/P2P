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
#define MAX 50 // Quantos IPs na lista
#define TAM 20 // Caracteres no IP
#define CHAVE "DiJqWHqKtiDgZySAv7ZX"

void * servidor(){
    
    int porta, nova_porta, tamanho, numbytes, i;
    int ping, logado, saiu;
    struct sockaddr_in endereco_meu;
    struct sockaddr_in endereco_cliente;
    char buffer[1000];
    char ips[50][20];
    char * ips_string = (char*) malloc(1000*sizeof(char));
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_cliente = (char*) malloc(20*sizeof(char));
    protocolo protoin;
    archive_def files[10];
    
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
    
    tamanho = sizeof(struct sockaddr_in);
    
    /*Teste*/
    insert_ip(ips, "111.222.333.444");
    insert_ip(ips, "2.3.8.444");
    insert_ip(ips, "199.5.55.5");
    for(i = 0; i < 10; i++){
        files[i].id = i;
        files[i].name = (char*) malloc(50*sizeof(char));
        sprintf(files[i].name, "arq%d.txt", i*3);
        files[i].size = (char*) malloc(20*sizeof(char));
        sprintf(files[i].size, "%d", i*30+215);
        files[i].http = (char*) malloc(50*sizeof(char));
        sprintf(files[i].http, "http://%s/%s", ip_meu, files[i].name);
        files[i].md5 = (char*) malloc(50*sizeof(char));
        strcpy(files[i].md5, "Breve.Aguarde!");
    }
    
/* Aguarda conexoes ***********************************************************/
    while (1){
        
        /*Fica esperando aqui*/
        nova_porta = accept(porta, (struct sockaddr*)&endereco_cliente, &tamanho);
        
        if(nova_porta==-1){
            perror("\n ::::: Erro: servidor: accept retornou erro\n");
            exit(1);
        }
        
        /* Pega o IP do cliente atual */
        strcpy(ip_cliente, inet_ntoa(endereco_cliente.sin_addr));
        
/* Recebe dados *******************************************************/
        ping = 0;
        //printf("\nservidor entrou no ping.");
        if((numbytes = recv(nova_porta, buffer, 999, 0)) == -1) {
            perror("\n ::::: Erro: Servidor nao conseguiu receber.\n");
            break;
        }
        buffer[numbytes] = '\0';
        //printf("\n ::::: Servidor recebeu: %s\n", buffer);
        protoin = set_proto(buffer);
/**********************************************************************/
        
        if(protoin.ok){
            if(!strcmp(protoin.command, "ping")){
                //printf("\n ::::: Servidor enviando pong...\n");
                if (send(nova_porta, pong(ip_meu, ip_cliente), 200, 0) == -1){
                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'pong'.");
                }
            }
            else{
                if(!strcmp(protoin.command, "authenticate")){
                    if(!strcmp(protoin.passport, CHAVE)){
                        //printf("\n ::::: Servidor enviando authenticate-back...\n");
                        if(send(nova_porta, authenticate_back(200, ip_meu, ip_cliente), 200, 0) == -1){
                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'authenticate-back'.");
                        }
                        insert_ip(ips, ip_cliente);
                    }
                    else{
                        send(nova_porta, authenticate_back(203, ip_meu, ip_cliente), 200, 0);
                    }
                }
                else{
                    if(find_ip(ips, ip_cliente)){
                        if(!strcmp(protoin.command, "agent-list")){
                            if(send(nova_porta, agent_list_back(200, get_ips_string(ips), ip_meu, ip_cliente), 200, 0) == -1){
                                perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-list-back'.");
                            }
                        }
                        else{
                            if(!strcmp(protoin.command, "archive-list")){
                                if(send(nova_porta, archive_list_back(200, files, ip_meu, ip_cliente), 200, 0) == -1){
                                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-list-back'.");
                                }
                            }
                            else{
                                if(!strcmp(protoin.command, "archive-request")){
                                    if(1/*tem_file(protoin.id)*/){
                                        if(send(nova_porta, archive_request_back(302, files[2], ip_meu, ip_cliente), 200, 0) == -1){
                                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-request-back'.");
                                        }
                                    }
                                    else{
                                        send(nova_porta, archive_request_back(404, files[0], ip_meu, ip_cliente), 200, 0);
                                    }
                                }
                                else{
                                    if(!strcmp(protoin.command, "end-connection")){
                                        saiu = 1;
                                        remove_ip(ips, ip_cliente);
                                    }
                                    else{
                                        //comando nao reconhecido
                                        send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 200, 0);
                                    }
                                }
                            }
                        }
                    }
                    else{
                        send(nova_porta, authenticate_back(401, ip_meu, ip_cliente), 200, 0);
                    }
                }
            }
        }
        else{
            //proto nao ok
            send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 200, 0);
        }
        
        /*if (fork()==0){ // se for o filho
            //printf("\nxxxxx fork == 0 xxxxx\n");
            close(porta); // o filho nao aceita conexoes a mais
            //close(nova_porta);
            //exit(0); // tao logo termine, o filho pode sair
        }
        else{
            //printf("\nxxxxx fork != 0 xxxxx\n");
        }*/
        close(nova_porta); // essa parte somente o pai executa
    }
}
