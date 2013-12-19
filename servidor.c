#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define PORTA_SERVIDOR 9876
#define MAX_THREADS 10 // Quantas conexoes simultaneas
#define MAX 50 // Quantos IPs na lista
#define TAM 20 // Caracteres no IP
#define CHAVE "DiJqWHqKtiDgZySAv7ZX"

int servidor(){
    
    int porta;
    struct sockaddr_in endereco_meu;
    
    /* cria socket. PF_INET define IPv4, SOCK_STREAM define TCP */
    porta = socket(PF_INET, SOCK_STREAM, 0);
    
    /* verifica se foi criado */
    if (porta == -1){
        perror("\n ::::: Erro: porta nao foi criada corretamente");
        //exit(1);
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
        //exit(1);
    }
    
    return porta;

}

/* Aguarda conexoes ***********************************************************/
void * start_connection(void* server_port){
    
    int nova_porta, tamanho, numbytes, i, repete;
    intptr_t porta;
    static int num_threads = 0;
    char ip_meu[20];
    char ip_cliente[20];
    char buffer[1000];
    static char ips[50][20];
    struct sockaddr_in endereco_cliente;
    pthread_t new_thread;
    protocolo protoin;
    static archive_def files[100];
    
    porta = (intptr_t) server_port;
    tamanho = sizeof(struct sockaddr_in);
    strcpy(ip_meu, get_my_ip());
    
    /*Teste*/
    insert_ip(1, ips, "111.222.333.444");
    insert_ip(1, ips, "2.3.8.444");
    insert_ip(1, ips, "199.5.55.5");
    for(i = 1; i <= 10; i++){
        files[i].id = i;
        sprintf(files[i].name, "arq%d.txt", i);
        sprintf(files[i].size, "%d", i);
        sprintf(files[i].http, "arquivo.txt");
        strcpy(files[i].md5, "Breve.Aguarde!");
    }
    /*Enviar quando solicitado arquivo inexistente*/
    files[0].id = 0;
    strcpy(files[0].name, "Arquivo nao existe");
    strcpy(files[0].size, "0");
    strcpy(files[0].http, ":(");
    strcpy(files[0].md5, " ");
    
    repete = 1;
    while(repete){
        
        /*Fica esperando aqui*/
        nova_porta = accept(porta, (struct sockaddr*)&endereco_cliente, &tamanho);
        
        /* Ao aceitar uma conexao, cria uma nova thread para esperar nova conexao. */
        /* Se o numero de threads chegou ao limite, nao abre uma nova, mas repete 
         * a atual, quando terminar de tratar a conexao corrente. */
        if(num_threads < MAX_THREADS){
            pthread_create(&new_thread, NULL, start_connection, (void*) porta);
            num_threads++;
            repete = 0;
        }
        
        if(nova_porta==-1){
            perror("\n ::::: Erro: servidor: accept retornou erro\n");
            exit(1);
        }
        
        /* Pega o IP do cliente atual */
        strcpy(ip_cliente, inet_ntoa(endereco_cliente.sin_addr));
        
/* Recebe dados *******************************************************/
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
                if (send(nova_porta, pong(ip_meu, ip_cliente), 999, 0) == -1){
                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'pong'.");
                }
            }
            else{
                if(!strcmp(protoin.command, "authenticate")){
                    if(!strcmp(protoin.passport, CHAVE)){
                        //printf("\n ::::: Servidor enviando authenticate-back...\n");
                        if(send(nova_porta, authenticate_back(200, ip_meu, ip_cliente), 999, 0) == -1){
                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'authenticate-back'.");
                        }
                        insert_ip(1, ips, ip_cliente);
                    }
                    else{
                        send(nova_porta, authenticate_back(203, ip_meu, ip_cliente), 999, 0);
                    }
                }
                else{
                    /* A partir daqui so aceita se o cliente estiver logado */
                    if(server_find_ip(ips, ip_cliente)){
                        if(!strcmp(protoin.command, "agent-list")){
                            if(send(nova_porta, agent_list_back(200, get_ips_string(ips), ip_meu, ip_cliente), 999, 0) == -1){
                                perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-list-back'.");
                            }
                        }
                        else{
                            if(!strcmp(protoin.command, "archive-list")){
                                if(send(nova_porta, archive_list_back(200, files, 10, ip_meu, ip_cliente), 999, 0) == -1){
                                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-list-back'.");
                                }
                            }
                            else{
                                if(!strcmp(protoin.command, "archive-request")){
                                    if(tem_arch(files, 11, protoin.file.id)){
                                        system("python -m SimpleHTTPServer & "); //inicializa servidor web
                                        if(send(nova_porta, archive_request_back(302, files[protoin.file.id], ip_meu, ip_cliente), 999, 0) == -1){
                                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-request-back'.");
                                        }
                                    }
                                    else{
                                        send(nova_porta, archive_request_back(404, files[0], ip_meu, ip_cliente), 999, 0);
                                    }
                                }
                                else{
                                    if(!strcmp(protoin.command, "end-connection")){
                                        remove_ip(1, ips, ip_cliente);
                                    }
                                    else{
                                        //comando nao reconhecido
                                        send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 999, 0);
                                    }
                                }
                            }
                        }
                    }
                    else{
                        send(nova_porta, authenticate_back(401, ip_meu, ip_cliente), 999, 0);
                    }
                }
            }
        }
        else{
            //proto nao ok
            send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 999, 0);
        }
    }
    /*if (fork()==0){ // se for o filho
        close(porta); // o filho nao aceita conexoes a mais
        //close(nova_porta);
        //exit(0); // tao logo termine, o filho pode sair
    }*/
    num_threads--;
    //fechar o servidor tambem
    close(nova_porta); // essa parte somente o pai executa
}
