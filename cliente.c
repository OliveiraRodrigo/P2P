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
    
    int i, j, seq, quit, porta_destino, numbytes, codigo;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    long addr_destino;
    char buffer[1000];
    char ** comando;
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_destino = (char*) malloc(20*sizeof(char));
    protocolo proto_in;
    
    strcpy(ip_meu, get_my_ip());
    
/* Recebe comandos enquanto um deles nao for "quit" ***************************/
    quit = 0;
    while(!quit){
        
        /* Prompt */
        printf("\n P2P:> ");
        
        /* Aloca espaco para 4 parametros com 50 caracteres cada */
        comando = (char**) malloc(4*sizeof(char*));
        for(i = 0; i < 4; i++){
            comando[i] = (char*) malloc(50*sizeof(char));
        }
        
        /* Recebe os parametros caracter a caracter */
        i = 0;
        j = 0;
        comando[i][j] = getchar();
        j++;
        while(comando[i][j-1] != '\n'){
            comando[i][j] = getchar();
            if(comando[i][j] == ' '){
                comando[i][j] = '\0'; // Descarta o ' '.
                j = 0;
                i++;
            }
            else{
                j++;
            }
        }
        comando[i][j-1] = '\0'; // Descarta o '\n'.
        
/* Comandos do cliente - Exemplos: (Colocar algo assim num comando "help")
 * 
 * try 123.321.1.2              (Manda um ping pra este ip e recebe um pong.)
 * 
 * login                        (Tenta autenticar-se com o mesmo ip. Se receber
 *                               authenticate-back codigo 200, envia agent-list,
 *                               recebe agent-list-back, insere na lista os
 *                               ips recebidos e retorna a lista pro usuario.)
 * 
 * list 159.4.4.4               (Envia archive-list para este ip. Se receber
 *                               archive-list-back, retorna a lista pro usuario.
 *                               Deve antes estar logado com este ip.)
 * 
 * down 2 159.4.4.4             (Envia arquive-request a 555.5.5.5 pedindo o arquivo de id:"2".)
 * 
 * quit                         (Envia um end-connection a todos os ips da lista.)
 * 
 */
        
        switch(qual_comando(comando[0])){
            /* try */
            case 0:
                seq = 0;
                addr_destino = inet_addr(comando[1]);
                if ((he=gethostbyaddr((char *) &addr_destino, sizeof(addr_destino), AF_INET)) == NULL) {
                    perror("\n P2P:> Erro: cliente nao conseguiu descobrir onde esta o servidor.\n");
                    //exit(1);
                    break;
                }
                
                if ((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                    perror("\n P2P:> Erro: cliente nao conseguiu criar porta\n");
                    //exit(1);
                    break;
                }
                
                /* prepara estrutura com endereco do servidor */
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
            /* login */
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
                    proto_in = set_proto(buffer);
                    set_ips_list(proto_in.back);
                    codigo = proto_in.status;
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
            /* list */
            case 2:
                break;
            /* down */
            case 3:
                break;
            /* get my IP */
            case 4:
                printf(" P2P:> %s", get_my_ip());
                break;
            /* quit */
            case 98:
                quit = 1;
                break;
            /* erro */
            case 99:
                printf("\n P2P:> Comando inexistente: '%s'", comando[0]);
                break;
        }
    }
    
    //close(sua_porta);
    //return 0;
    
}
