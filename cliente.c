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

void * cliente(){
    
    int i, j, pong, logado, logout, seq, quit;
    int porta_destino, numbytes, codigo;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    long addr_destino, temp_addr;
    char buffer[1000];
    char ips[50][20];
    char ** comando;
    char ip_meu[20];
    char ip_destino[20];
    protocolo protoin;
    archive_def * files;
    
    /* Aloca espaco para 4 parametros com 50 caracteres cada */
    comando = (char**) malloc(4*sizeof(char*));
    for(i = 0; i < 4; i++){
        comando[i] = (char*) calloc(50,sizeof(char));
    }
    
    /* Detecta o IP local e o armazena em 'ip_meu' */
    strcpy(ip_meu, get_my_ip());
    
/* Recebe comandos enquanto um deles nao for "quit" ***************************/
    quit = 0;
    while(!quit){
        
        comando = get_command();
        if(!run_command(comando, ip_meu, &quit)){
            
            if(!strcmp(comando[0], "try")){
                
                strcpy(ip_destino, comando[1]);
                porta_destino = porta(comando[1]);
                if(porta_destino != -1){
                    
                    printf("\n P2P:> Enviando ping...");
                    //printf("\n P2P:> %s", ping(ip_meu, ip_destino));
                    
                    if(send(porta_destino, ping(ip_meu, ip_destino), 999, 0) != -1){
                        
                        if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                            
                            buffer[numbytes] = '\0';
                            //printf("\n P2P:> Cliente recebeu: %s", buffer);
                            protoin = set_proto(buffer);
                            
                            //Testa se recebeu um pong ok.
                            if(protoin.ok && !strcmp(protoin.command, "pong")){
                            //if(!strcmp(buffer, pong(ip_destino, ip_meu))){
                                pong = 1;
                                printf("\n P2P:> %s respondeu corretamente.\n", ip_destino);
                            }
                            else{
                                printf("\n P2P:> Erro: %s retornou %s, codigo %d.\n", ip_destino, protoin.command, protoin.status);
                            }
                        }
                        else{
                            perror("\n P2P:> Erro: nao conseguiu receber 'pong'\n");
                        }
                    }
                    else{
                        perror("\n P2P:> Erro: nao conseguiu enviar 'ping'\n");
                    }
                    close(porta_destino);
                }
            }
            else{
                if(!strcmp(comando[0], "login")){
                    
                    strcpy(ip_destino, comando[1]);
                    porta_destino = porta(comando[1]);
                    if(porta_destino != -1){
                    
                        printf("\n P2P:> Enviando authenticate...");
                        //printf("\n P2P:> %s", authenticate(CHAVE, ip_meu, ip_destino));
                        
                        if(send(porta_destino, authenticate(CHAVE, ip_meu, ip_destino), 999,0) != -1){
                            
                            if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                                
                                buffer[numbytes] = '\0';
                                //printf("\n P2P:> Cliente recebeu: %s", buffer);
                                protoin = set_proto(buffer);
                                
                                //Testa se recebeu um authenticate-back ok.
                                if(protoin.ok){
                                    if(!strcmp(protoin.command, "authenticate-back")){
                                        if(protoin.status == 200){
                                            printf("\n P2P:> Autenticacao com %s aceita.\n", ip_destino);
                                            //Salva o IP para desconectar ao sair.
                                            insert_ip(0, ips, ip_destino);
                                        }
                                        else{
                                            printf("\n P2P:> Erro: autenticacao com %s falhou. Codigo %d.\n", ip_destino, protoin.status);
                                        }
                                    }
                                    else{
                                        printf("\n P2P:> Erro: %s retornou %s, codigo %d.\n", ip_destino, protoin.command, protoin.status);
                                    }
                                }
                                else{
                                    printf("\n P2P:> Erro: %s retornou um protocolo incompativel.\n", ip_destino);
                                }
                            }
                            else{
                                perror("\n P2P:> Erro: nao conseguiu receber 'authenticate-back'\n");
                            }
                        }
                        else{
                            perror("\n P2P:> Erro: nao conseguiu enviar 'authenticate'\n");
                        }
                        close(porta_destino);
                    }
                }
                else{
                    if(!strcmp(comando[0], "list-users")){
                        
                        strcpy(ip_destino, comando[1]);
                        porta_destino = porta(comando[1]);
                        if(porta_destino != -1){
                            
                            printf("\n P2P:> Enviando agent-list...");
                            //printf("\n P2P:> %s", agent_list(ip_meu, ip_destino));
                            
                            if(send(porta_destino, agent_list(ip_meu, ip_destino), 999,0) != -1){
                                
                                if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                                    
                                    buffer[numbytes] = '\0';
                                    //printf("\n P2P:> Cliente recebeu: %s", buffer);
                                    protoin = set_proto(buffer);
                                    
                                    //Testa se recebeu um agent-list-back ok.
                                    if(protoin.ok){
                                        if(!strcmp(protoin.command, "agent-list-back")){
                                            if(protoin.status == 200){
                                                printf("\n P2P:> IPs: %s\n", protoin.back);
                                            }
                                            else{
                                                printf("\n P2P:> Erro: %s informou codigo %d.\n", ip_destino, protoin.status);
                                            }
                                        }
                                        else{
                                            printf("\n P2P:> Erro: %s retornou %s, codigo %d.\n", ip_destino, protoin.command, protoin.status);
                                        }
                                    }
                                    else{
                                        printf("\n P2P:> Erro: %s retornou um protocolo incompativel.\n", ip_destino);
                                    }
                                }
                                else{
                                    perror("\n P2P:> Erro: nao conseguiu receber 'agent-list-back'\n");
                                }
                            }
                            else{
                                perror("\n P2P:> Erro: nao conseguiu enviar 'agent-list'\n");
                            }
                            close(porta_destino);
                        }
                    }
                    else{
                        if(!strcmp(comando[0], "list-files")){
                            
                            strcpy(ip_destino, comando[1]);
                            porta_destino = porta(comando[1]);
                            if(porta_destino != -1){
                                
                                printf("\n P2P:> Enviando archive-list...");
                                //printf("\n P2P:> %s", archive_list(ip_meu, ip_destino));
                                
                                if(send(porta_destino, archive_list(ip_meu, ip_destino), 999,0) != -1){
                                    
                                    if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                                        
                                        buffer[numbytes] = '\0';
                                        //printf("\n P2P:> Cliente recebeu: %s", buffer);
                                        protoin = set_proto(buffer);
                                        
                                        //Testa se recebeu um archive-list-back ok.
                                        if(protoin.ok){
                                            if(!strcmp(protoin.command, "archive-list-back")){
                                                if(protoin.status == 200){
                                                    printf("\n P2P:> Arquivos de %s:\n\n\"%s\"\n", ip_destino, protoin.back);
                                                }
                                                else{
                                                    printf("\n P2P:> Erro: %s informou codigo %d.\n", ip_destino, protoin.status);
                                                }
                                            }
                                            else{
                                                printf("\n P2P:> Erro: %s retornou %s, codigo %d.\n", ip_destino, protoin.command, protoin.status);
                                            }
                                        }
                                        else{
                                            printf("\n P2P:> Erro: %s retornou um protocolo incompativel.\n", ip_destino);
                                        }
                                    }
                                    else{
                                        perror("\n P2P:> Erro: nao conseguiu receber 'archive-list-back'\n");
                                    }
                                }
                                else{
                                    perror("\n P2P:> Erro: nao conseguiu enviar 'archive-list'\n");
                                }
                                close(porta_destino);
                            }
                        }
                        else{
                            if(!strcmp(comando[0], "down")){
                                
                                strcpy(ip_destino, comando[2]);
                                porta_destino = porta(comando[2]);
                                if(porta_destino != -1){
                                    
                                    printf("\n P2P:> Enviando archive-request...");
                                    //printf("\n P2P:> %s", archive_request(comando[1], ip_meu, ip_destino));
                                    
                                    if(send(porta_destino, archive_request(comando[1], ip_meu, ip_destino), 999,0) != -1){
                                        
                                        if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                                            
                                            buffer[numbytes] = '\0';
                                            printf("\n P2P:> Cliente recebeu: %s", buffer);
                                            protoin = set_proto(buffer);
                                            
                                            //Testa se recebeu um archive-list-back ok.
                                            if(protoin.ok){
                                                if(!strcmp(protoin.command, "archive-request-back")){
                                                    if(protoin.status == 302){
                                                        printf("\n P2P:> %s enviou os dados do arquivo %d:\n\n\t Link: %s\n\t MD5:  %s\n",
                                                                ip_destino,
                                                                protoin.file.id,
                                                                protoin.file.http,
                                                                protoin.file.md5);
                                                    }
                                                    else{
                                                        printf("\n P2P:> Erro: %s informou codigo %d.\n", ip_destino, protoin.status);
                                                    }
                                                }
                                                else{
                                                    printf("\n P2P:> Erro: %s retornou %s, codigo %d.\n", ip_destino, protoin.command, protoin.status);
                                                }
                                            }
                                            else{
                                                printf("\n P2P:> Erro: %s retornou um protocolo incompativel.\n", ip_destino);
                                            }
                                        }
                                        else{
                                            perror("\n P2P:> Erro: nao conseguiu receber 'archive-request-back'\n");
                                        }
                                    }
                                    else{
                                        perror("\n P2P:> Erro: nao conseguiu enviar 'archive-request'\n");
                                    }
                                    close(porta_destino);
                                }
                            }
                            else{
                                if(!strcmp(comando[0], "logout")){
                                    
                                    strcpy(ip_destino, comando[1]);
                                    porta_destino = porta(comando[1]);
                                    if(porta_destino != -1){
                                        
                                        printf("\n P2P:> Enviando end-connection...");
                                        //printf("\n P2P:> %s", end-connection(ip_meu, ip_destino));
                                        
                                        if(send(porta_destino, end_connection(ip_meu, ip_destino), 999,0) != -1){
                                            printf("\n P2P:> Desconectado de %s.\n", ip_destino);
                                        }
                                        else{
                                            perror("\n P2P:> Erro: nao conseguiu enviar 'end-connection'\n");
                                        }
                                        //Remove o IP da lista, pois, ao sair, deste eu ja estou desconectado.
                                        remove_ip(0, ips, ip_destino);
                                        close(porta_destino);
                                    }
                                }
                                else{
                                    printf("\n P2P:> Comando inexistente: '%s'\n", comando[0]);
                                }
                            }
                        }
                    }
                }
            }
        }
        if(quit){
            //Envia end-connection para todos os IPs com quem estou conectado.
            for(i = 0; i < client_ips_size(0); i++){
                send(porta_destino, end_connection(ip_meu, ips[i]), 999,0);
            }
        }
    }
}

int porta(char * ip_destino){
	
    int porta_destino, numbytes, codigo;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    long addr_destino, temp_addr;
    //char * ip_destino = (char*) malloc(20*sizeof(char));
	
                    //temp_addr = addr_destino;
                    addr_destino = inet_addr(ip_destino);
                    
                    if((he=gethostbyaddr((char *) &addr_destino, sizeof(addr_destino), AF_INET)) == NULL) {
                        printf("\n P2P:> Erro: Nao foi possivel localizar '%s'.\n", ip_destino);
                        //addr_destino = temp_addr;
                        return -1;
                    }
                    
                    if((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                        perror("\n P2P:> Erro: Nao foi possivel criar a porta\n");
                        return -1;
                    }
                    
                    // prepara estrutura com endereco do servidor
                    endereco_destino.sin_family = AF_INET; 
                    endereco_destino.sin_port = htons(PORTA_SERVIDOR);
                    endereco_destino.sin_addr = *((struct in_addr *)he->h_addr);
                    memset(&(endereco_destino.sin_zero), '\0', 8);
                    //ip_destino = inet_ntoa(endereco_destino.sin_addr);
                    
                    if(connect(porta_destino,
                      (struct sockaddr *)&endereco_destino,
                      sizeof(struct sockaddr)) == -1) {
                        perror("\n P2P:> Erro: conectando no servidor\n");
                        return -1;
                    }
                    return porta_destino;
}
