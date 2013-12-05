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
    
    int i, j, pong, logado, logout, seq, esc_sessao, quit;
    int porta_destino, numbytes, codigo;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    long addr_destino, temp_addr;
    char buffer[1000];
    char ** comando;
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_destino = (char*) malloc(20*sizeof(char));
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
        
/* Envia ping, espera pong ****************************************************/
        pong = 0;
        while(!pong && !quit){
            printf("\nentrou no ping");
            comando = get_command();
            
            if(!run_command(comando, ip_meu, &esc_sessao, &quit)){
                
                if(!strcmp(comando[0], "try")){
// Se voltar pra ca, ele cria uma nova porta, entao nao conecta mais com o q estava antes                    
                    temp_addr = addr_destino;
                    addr_destino = inet_addr(comando[1]);
                    
                    if((he=gethostbyaddr((char *) &addr_destino, sizeof(addr_destino), AF_INET)) == NULL) {
                        printf("\n P2P:> Erro: Nao foi possivel localizar '%s'.\n", comando[1]);
                        addr_destino = temp_addr;
                        break;
                    }
                    
                    if((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                        perror("\n P2P:> Erro: Nao foi possivel criar a porta\n");
                        break;
                    }
                    
                    // prepara estrutura com endereco do servidor
                    endereco_destino.sin_family = AF_INET; 
                    endereco_destino.sin_port = htons(PORTA_SERVIDOR);
                    endereco_destino.sin_addr = *((struct in_addr *)he->h_addr);
                    memset(&(endereco_destino.sin_zero), '\0', 8);
                    ip_destino = inet_ntoa(endereco_destino.sin_addr);
                    
                    if(connect(porta_destino,
                      (struct sockaddr *)&endereco_destino,
                      sizeof(struct sockaddr)) == -1) {
                        perror("\n P2P:> Erro: conectando no servidor\n");
                        break;
                    }
                    
                    printf("\n P2P:> Enviando ping...");
                    //printf("\n P2P:> %s", ping(ip_meu, ip_destino));
                    if(send(porta_destino, ping(ip_meu, ip_destino), 200, 0) == -1){
                        perror("\n P2P:> Erro: nao conseguiu enviar 'ping'");
                    }
                    
                    if((numbytes = recv(porta_destino, buffer, 999, 0)) == -1) {
                        perror("\n P2P:> Erro: nao conseguiu receber 'pong'\n");
                        break;
                    }
                    
                    buffer[numbytes] = '\0';
                    
                    //printf("\n P2P:> Cliente recebeu: %s", buffer);
                    
                    //Testa se recebeu um pong ok.
                    protoin = set_proto(buffer/*"{protocol:\"pcmj\", command:\"ping\", sender:\"1.1.1.1\", receptor:\"2.2.2.2\"}"*/);
                    if(protoin.ok && !strcmp(protoin.command, "pong")){
                        pong = 1;
                        printf("\n P2P:> %s respondeu corretamente.\n", ip_destino);
                    }
                    else{
                        printf("\n P2P:> Erro: %s retornou codigo %d.\n", ip_destino, protoin.status);
                    }
                }
                else{
                    printf("\n P2P:> Comando inesperado: '%s'\n", comando[0]);
                }
            }
        //}
        
/* Envia authenticate, espera authenticate-back *******************************/
        logado = 0;
        esc_sessao = 0;
        while(pong && !logado && !esc_sessao){
            printf("\nentrou no login");
            comando = get_command();
            
            if(!run_command(comando, ip_meu, &esc_sessao, &quit)){
                
                if(!strcmp(comando[0], "login")){
                    printf("\n P2P:> Enviando authenticate...");
                    //printf("\n P2P:> %s", authenticate(ip_meu, ip_destino));
                    if(send(porta_destino, authenticate(CHAVE, ip_meu, ip_destino), 200,0) == -1){
                        perror("\n P2P:> Erro: nao conseguiu enviar 'authenticate'");
                    }
                    if((numbytes = recv(porta_destino, buffer, 999, 0)) == -1) {
                        perror("\n P2P:> Erro: nao conseguiu receber 'pong'\n");
                        break;
                    }
                    
                    buffer[numbytes] = '\0';
                    
                    //printf("\n P2P:> Cliente recebeu: %s", buffer);
                    
                    //Testa se recebeu um authenticate-back ok.
                    protoin = set_proto(buffer);
                    if(protoin.ok){
                        if(!strcmp(protoin.command, "authenticate-back")){
                            if(protoin.status == 200){
                                logado = 1;
                                printf("\n P2P:> Autenticacao com %s aceita.\n", ip_destino);
                            }
                            else{
                                printf("\n P2P:> Erro: autenticacao com %s falhou. Codigo %d.\n", ip_destino, protoin.status);
                            }
                        }
                        else{
                            printf("\n P2P:> Eu esperava 'authenticate-back'. %s enviou '%s'.\n", ip_destino, protoin.command);
                        }
                    }
                    else{
                        printf("\n P2P:> Erro: %s retornou um protocolo incompativel.\n", ip_destino);
                    }
                }
                /*else{
                if(!strcmp(comando[0], "try")){
                    send(porta_destino, ping(ip_meu, ip_destino), 200, 0);
                    numbytes = recv(porta_destino, buffer, 999, 0);
                    buffer[numbytes] = '\0';
                    printf("\n P2P:> Cliente recebeu: %s", buffer);
                }*/
                else{
                    printf("\n P2P:> Comando inesperado: '%s'\n", comando[0]);
                }
            //}
            }
            
///////////Colocar AQUI a parte "Executa qualquer comando".
            
        }//while do login
        printf("\nsaiu do login");
        pong = 0;
        }//while do ping
        printf("\nsaiu do ping");

/* Executa qualquer comando ***************************************************/
/*
  logout = 0
  while(!logout){
	
	aceita comandos do usuário
	
	if(comando interno ou login)
		executa função interna ou mostra já está logado
	
	else if(comando == list-users)
		send(agent-list)
		recv(buffer)
		if(buffer tem agent-list-back 200)
			armazena os ips recebidos na lista local
			mostra lista atualizada
		else
			mostra o erro recebido
	else if(comando == list-files)
		send(arqchive-list)
		recv(buffer)
		if(buffer tem archive-list-back 200)
			mostra lista de arquivos recebida
		else
			mostra o erro recebido
	else if(comando == down <ID>)
		send(arqchive-request <ID>)
		recv(buffer)
		if(buffer tem archive-request-back 302)
			baixa o arquivo (mas como?)
		else
			mostra o erro recebido
	else if(comando == logout)
		logout = 1
		send(end-connection)
  }
*/







        /*switch(qual_comando(comando[0])){
            // try //
            case 0:
                seq = 0;
                addr_destino = inet_addr(comando[1]);
                if ((he=gethostbyaddr((char *) &addr_destino, sizeof(addr_destino), AF_INET)) == NULL) {
                    printf("\n P2P:> Erro: Nao foi possivel localizar %s.\n", addr_destino);
                    //exit(1);
                    break;
                }
                
                if ((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                    perror("\n P2P:> Erro: Nao foi possivel criar a porta\n");
                    //exit(1);
                    break;
                }
                
                // prepara estrutura com endereco do servidor
                endereco_destino.sin_family = AF_INET; 
                endereco_destino.sin_port = htons(PORTA_SERVIDOR);
                endereco_destino.sin_addr = *((struct in_addr *)he->h_addr);
                memset(&(endereco_destino.sin_zero), '\0', 8);
                ip_destino = inet_ntoa(endereco_destino.sin_addr);
                
                if (connect(porta_destino,
                   (struct sockaddr *)&endereco_destino,
                   sizeof(struct sockaddr)) == -1) {
                    perror("\n P2P:> Erro: conectando no servidor\n");
                    //exit(1);
                    break;
                }
                
                printf("\n P2P:> Enviando ping...");
                //printf("\n P2P:> %s", ping(ip_meu, ip_destino));
                if (send(porta_destino, ping(ip_meu, ip_destino), 200, 0) == -1){
                    perror("\n P2P:> Erro: nao conseguiu enviar 'ping'");
                }
                
                if ((numbytes = recv(porta_destino, buffer, 254, 0)) == -1) {
                    perror("\n P2P:> Erro: nao conseguiu receber 'pong'\n");
                    //exit(1);
                    break;
                }
                
                buffer[numbytes] = '\0';
                
                //printf("\n P2P:> Cliente recebeu: %s", buffer);
                
                // Testa se o recebido foi um pong.
                if(!strcmp(buffer, pong(ip_destino, ip_meu))){
                    printf("\n P2P:> Conexao com %s aceita.\n", ip_destino);
                    seq = 1;
                }
                else{
                    printf("\n P2P:> Erro: %s nao enviou um 'pong' compativel.\n", ip_destino);
                }
                break;
            // login //
            case 1:
                if(seq < 1){
                    printf("\n P2P:> Primeiro execute 'try <IP>'.\n");
                    break;
                }
                printf("\n P2P:> Autenticando-se com %s...", ip_destino);
                //printf("\n P2P:> %s", authenticate(CHAVE, ip_meu, ip_destino));
                if (send(porta_destino, authenticate(CHAVE, ip_meu, ip_destino), 200, 0) == -1){
                    perror("\n P2P:> Erro: nao conseguiu enviar autenticacao.");
                }
                
                if ((numbytes = recv(porta_destino, buffer, 254, 0)) == -1) {
                    perror("\n P2P:> Erro: nao obteve resposta.\n");
                    //exit(1);
                    break;
                }
                
                buffer[numbytes] = '\0';
                
                //printf("\n P2P:> Cliente recebeu: %s", buffer);
                
                // Testa se o recebido foi um authenticate-back.
                if(!strcmp(buffer, authenticate_back(200, ip_destino, ip_meu))){
                    printf("\n P2P:> Autenticacao com %s aceita.\n", ip_destino);
                    seq = 2;
                    // Solicita um agent-list
                    printf("\n P2P:> Solicitando agent-list...\n");
                    if (send(porta_destino, agent_list(ip_meu, ip_destino), 200, 0) == -1){
                        perror("\n P2P:> Erro: nao conseguiu enviar 'agent-list'.");
                    }
                    if ((numbytes = recv(porta_destino, buffer, 999, 0)) == -1) {
                        perror("\n P2P:> Erro: nao obteve resposta.\n");
                        //exit(1);
                        break;
                    }
                    buffer[numbytes] = '\0';
                    //printf("\n P2P:> Cliente recebeu: %s.\n", buffer);
                    // Testa se o recebido foi um agent-list-back.
                    protoin = set_proto(buffer);
                    //set_ips_list(proto_in.back);
                    codigo = protoin.status;
                    if(codigo == 200){
                        printf("\n P2P:> Lista de IPs recebida e inserida na lista local.\n");
                    }
                    else{
                        printf("\n P2P:> Erro: %s retornou codigo %d.\n", ip_destino, codigo);
                    }
                }
                else{
                    printf("\n P2P:> Erro: %s nao autenticou.\n", ip_destino);
                }
                break;
            // list //
            case 2:
                break;
            // down //
            case 3:
                break;
            // get my IP //
            case 4:
                strcpy(ip_meu, get_my_ip());
                printf(" P2P:> %s", ip_meu);
                break;
            // informo o meu IP, caso get_my_ip nao de certo //
            case 5:
                strcpy(ip_meu, comando[1]);
                printf(" P2P:> Ok: usando %s como meu IP.", ip_meu);
                break;
            // help //
            case 97:
                help();
                break;
            // quit //
            case 98:
                quit = 1;
                break;
            // erro //
            case 99:
                printf("\n P2P:> Comando inexistente: '%s'", comando[0]);
                break;
        }*/
    }
    //Aqui, quit == 1, entao enviar end-conection para os IPs da lista.
    
    //close(sua_porta);
    //return 0;
    
}
