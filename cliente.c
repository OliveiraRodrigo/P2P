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

void * cliente(/*char** parametros*/){
    
    int i, j, quit, porta_destino, numbytes;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    long addr_destino;
    char buffer[255];
    char ** comando;
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_destino = (char*) malloc(20*sizeof(char));
    
    ip_meu = get_my_ip();
    
/* Recebe comandos enquanto um deles nao for "quit" ***************************/
    quit = 0;
    while(!quit){
        
        /* Prompt */
        printf("\n P2P:> ");
        
        /* Aloca espaco para 4 parametros com 50 caracteres cada */
        comando = (char**) malloc(10*sizeof(char));
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
 * login minhasenha 123.321.1.2 (Tenta autenticar-se com este ip. Se receber
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
                
                printf("\n P2P:> Cliente enviando ping...");
                printf("\n P2P:> %s", ping(ip_meu, ip_destino));
                if (send(porta_destino, ping(ip_meu, ip_destino), 200, 0) == -1){
                    perror("\n P2P:> Erro: nao conseguiu mandar mensagem");
                }
                
                if ((numbytes=recv(porta_destino, buffer, 254, 0)) == -1) {
                    perror("\n P2P:> Erro: recv no peer cliente\n");
                    //exit(1);
                    break;
                }
                
                buffer[numbytes] = '\0';
                
                printf("\n P2P:> Cliente recebeu: %s", buffer);
                
                // Testar se o recebido foi um pong e retornar um OK pro usuario, neste caso.
                
                break;
            /* login */
            case 1:
                break;
            /* list */
            case 2:
                break;
            /* down */
            case 3:
                break;
            /* quit */
            case 4:
                quit = 1;
                break;
            case 9:
                printf("\n P2P:> Comando inexistente: %s", comando[0]);
                break;
        }
    }
    
    //close(sua_porta);
    //return 0;
    
}

char * get_my_ip(){
    
    char host_meu[128];
    struct hostent *he;
    struct sockaddr_in addr;
    char * ip = (char*) malloc(20*sizeof(char));
    
    gethostname(host_meu, sizeof host_meu);
    //printf("\n meu host: %s\n", host_meu);
    if ((he=gethostbyname(host_meu)) == NULL) {
        perror("\nerro: could not gethostbyname\n");
        exit(1);
    }
    addr.sin_family = PF_INET;
    addr.sin_port = htons(PORTA_SERVIDOR);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(addr.sin_zero), '\0', 8);
    ip = inet_ntoa(addr.sin_addr);
    //printf("\n meu ip: %s ", ip_meu);
    return ip;
}

int qual_comando(char * comando){
    
    if(!strcmp(comando, "try"))
        return 0;
    if(!strcmp(comando, "login"))
        return 1;
    if(!strcmp(comando, "list"))
        return 2;
    if(!strcmp(comando, "down"))
        return 3;
    if(!strcmp(comando, "q")) //"quit"
        return 4;
    return 9;
}

char * ping(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    //printf("\ndentro do ping: meu: %s ", ip_meu);
    //printf("\ndentro do ping: dest: %s ", ip_destino);
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"ping\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}

char * agent_list(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"agent-list\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}

char * authenticate(char * pass, char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"authenticate\", "
                   "passport:\"%s\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", pass, ip_meu, ip_destino);
    
    return saida;
}

char * archive_list(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"archive-list\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}

char * archive_request(char * arch_id, char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"archive-request\", "
                   "id:\"%s\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", arch_id, ip_meu, ip_destino);
    
    return saida;
}

char * end_connection(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"end-connection\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}
