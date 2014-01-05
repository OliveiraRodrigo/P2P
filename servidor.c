#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include "comandos.h"

#define MAX_THREADS 3 // Quantas conexoes simultaneas
#define CHAVE "DiJqWHqKtiDgZySAv7ZX"

intptr_t servidor(intptr_t porta_servidor){
    
    intptr_t porta;
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
    endereco_meu.sin_port = htons(porta_servidor);
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
    
    int tamanho, numbytes, repete, fileCounter, i;
    intptr_t porta, nova_porta;
    static int num_threads = 0;
    char ip_meu[20];
    char ip_cliente[20];
    char buffer[10000];
    static IPs ips;
    struct sockaddr_in endereco_cliente;
    pthread_t new_thread, http_thread;
    protocolo protoin;
    archive_def files[100];
    
    porta = (intptr_t) server_port;
    tamanho = sizeof(struct sockaddr_in);
    strcpy(ip_meu, get_my_ip());
    
    /*Teste
    ips_list(INSERT, SERVER, "111.222.333.444", NULL);
    ips_list(INSERT, SERVER, "2.3.8.444", NULL);
    ips_list(INSERT, SERVER, "199.5.55.5", NULL);*/
    
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
                        ips_list(INSERT, SERVER, ip_cliente, NULL);
                    }
                    else{
                        send(nova_porta, authenticate_back(203, ip_meu, ip_cliente), 999, 0);
                    }
                }
                else{
                    /* A partir daqui so aceita se o cliente estiver logado */
                    if(ips_list(FIND, SERVER, ip_cliente, NULL)){
                        if(!strcmp(protoin.command, "agent-list")){
                            ips_list(GET, SERVER, NULL, ips);
                            if(send(nova_porta, agent_list_back(200, get_ips_string(ips), ip_meu, ip_cliente), 999, 0) == -1){
                                perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-list-back'.");
                            }
                        }
                        else{
                            if(!strcmp(protoin.command, "archive-list")){
                                fileCounter = setFileList("shared", files);
                                if(send(nova_porta, archive_list_back(200, files, fileCounter, ip_meu, ip_cliente), 9999, 0) == -1){
                                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-list-back'.");
                                }
                            }
                            else{
                                if(!strcmp(protoin.command, "archive-request")){
                                    fileCounter = setFileList("shared", files);
                                    if(tem_arch(files, fileCounter, protoin.file.id)){
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
                                        ips_list(REMOVE, SERVER, ip_cliente, NULL);
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

    
int setFileList(char folder[100], archive_def * files){
    
    int i, fileCounter;
    char path[200];
    struct dirent *lsdir;
    float fileSize;
    DIR *dir;
    FILE *fp;
    
    i = 1;
    fileCounter = 0;
    dir = opendir(folder);
    while((lsdir = readdir(dir)) != NULL){
        if(strcmp(lsdir->d_name, ".") && strcmp(lsdir->d_name, "..")){
            //printf("<%s>\n", lsdir->d_name);
            fileCounter++;
            files[i].id = i;
            sprintf(files[i].name, "%s", lsdir->d_name);
            
            /* tamanho do arquivo */
            sprintf(path, "%s/%s", folder, lsdir->d_name);
            fp = fopen(path, "rb");
            fseek(fp, 0, SEEK_END);
            fileSize = ftell(fp);
            fclose(fp);
            
            sprintf(files[i].size, "%1.2f", fileSize/1024);
            sprintf(files[i].http, "%s", lsdir->d_name);
            strcpy(files[i].md5, (char*)MD5(path));
            i++;
        }
    }
    closedir(dir);
    
    /*Enviar quando solicitado arquivo inexistente*/
    files[0].id = 0;
    strcpy(files[0].name, "Arquivo nao existe");
    strcpy(files[0].size, "0");
    strcpy(files[0].http, ":(");
    strcpy(files[0].md5, " ");
    
    return fileCounter;
    
}
