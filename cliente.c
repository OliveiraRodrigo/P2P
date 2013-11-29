#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define PORTA_SERVIDOR 9876

void * cliente(char** parametros){
    
    char * server_name = (char*) malloc(20*sizeof(char));
    int porta_minha, porta_destino, numbytes;
    struct sockaddr_in endereco_meu;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    char * ip_meu = (char*) malloc(20*sizeof(char));
    char * ip_destino = (char*) malloc(20*sizeof(char));
    char buffer[255];
        
/////////////////////////////////////////////////////////
    endereco_meu.sin_family = AF_INET;
    endereco_meu.sin_port = htons(PORTA_SERVIDOR);
    endereco_meu.sin_addr.s_addr = INADDR_ANY;
    ip_meu = inet_ntoa(endereco_meu.sin_addr);

    memset(&(endereco_meu.sin_zero), '\0', 8);
/////////////////////////////////////////////////////////
    
    switch(qual_comando(parametros[0])){
        case 0:
            /* passe o nome do host onde esta o servidor e essa funcao retorna um endereco */
            server_name = "PPGEnfSecretari";
            if ((he=gethostbyname(server_name)) == NULL) {
            /*if ((he=gethostbyaddr(seu_endereco.sin_addr.s_addr, 200, AF_INET)) == NULL) {*/
            //if ((he=getservent()) == NULL) {
                perror("\n P2P:> erro: cliente nao conseguiu descobrir aonde esta o servidor\n");
                exit(1);
            }
            
            if ((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("\n P2P:> erro: cliente nao conseguiu criar porta\n");
                exit(1);
            }
            
            /* prepara estrutura com endereco do servidor */
            endereco_destino.sin_family = AF_INET; 
            endereco_destino.sin_port = htons(PORTA_SERVIDOR);
            endereco_destino.sin_addr = *((struct in_addr *)he->h_addr);
            memset(&(endereco_destino.sin_zero), '\0', 8);
            ip_destino = inet_ntoa(endereco_destino.sin_addr);
            //ip_destino = "127.0.0.1";
            
            if (connect(porta_destino,
               (struct sockaddr *)&endereco_destino,
               sizeof(struct sockaddr)) == -1) {
                perror("\n P2P:> erro: conectando no servidor\n");
                exit(1);
            }
            
            printf("\n P2P:> Cliente enviando ping...");
            if (send(porta_destino, ping(ip_meu, parametros[1]/*ip_destino*/), 200, 0) == -1)
            perror("\n P2P:> erro: nao conseguiu mandar mensagem");
            
            if ((numbytes=recv(porta_destino, buffer, 254, 0)) == -1) {
                perror("\n P2P:> erro: recv no peer cliente\n");
                exit(1);
            }
            
            buffer[numbytes] = '\0';

            printf("\n P2P:> Cliente recebeu: %s", buffer);
            break;
        case 1:
            break;
        }




	while(1);
        //close(sua_porta);

	//return 0;

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
    if(!strcmp(comando, "quit"))
        return 4;
}

char * ping(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"ping\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}
