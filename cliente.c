#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define PORTA_SERVIDOR 9876

void * cliente(/*char** parametros*/){
    
    char * server_name = (char*) malloc(20*sizeof(char));
    char ** comando;
    int porta_minha, porta_destino, numbytes;
    struct sockaddr_in endereco_meu;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    char * ip_meu = (char*) malloc(20*sizeof(char));
    char * ip_destino = (char*) malloc(20*sizeof(char));
    char buffer[255];
    int i, j, quit;
    
/////////////////////////////////////////////////////////
    endereco_meu.sin_family = AF_INET;
    endereco_meu.sin_port = htons(PORTA_SERVIDOR);
    endereco_meu.sin_addr.s_addr = INADDR_ANY;
    ip_meu = inet_ntoa(endereco_meu.sin_addr);
    printf(" meu ip: %s ", ip_meu);
    
    memset(&(endereco_meu.sin_zero), '\0', 8);
/////////////////////////////////////////////////////////
    
    quit = 0;
    while(!quit){
        
    printf("\n\n P2P:> ");
    
     /*Aloca espaco para 4 parametros com 50 caracteres cada*/ 
    comando = (char**) malloc(10*sizeof(char));
    for(i = 1; i <= 4; i++){
        comando[i] = (char*) malloc(50*sizeof(char));
    }
        
    i = 1;
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
    
    /* Comandos do cliente - Exemplos:
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

    switch(qual_comando(comando[1])){
        /* try */
        case 0:
            strcpy(ip_destino,comando[2]);
            /* passe o nome do host onde esta o servidor e essa funcao retorna um endereco */
            strcpy(server_name,"Rodrigo-PC");
            //if ((he=gethostbyname(server_name)) == NULL) {
            //if ((he=gethostbyaddr(ip_destino, 200, AF_INET)) == NULL) {
            //   perror("\n P2P:> erro: cliente nao conseguiu descobrir aonde esta o servidor\n");
            //    exit(1);
            //}
            if ((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("\n P2P:> erro: cliente nao conseguiu criar porta\n");
                exit(1);
            }
            
            /* prepara estrutura com endereco do servidor */
            endereco_destino.sin_family = AF_INET; 
            endereco_destino.sin_port = htons(PORTA_SERVIDOR);
            //endereco_destino.sin_addr = *((struct in_addr *)he->h_addr);
            endereco_destino.sin_addr = *((struct in_addr *)inet_addr(ip_destino));
            memset(&(endereco_destino.sin_zero), '\0', 8);
            //ip_destino = inet_ntoa(endereco_destino.sin_addr);
            printf("--%s--", ip_destino);
            //ip_destino = "127.0.0.1";
            
            if (connect(porta_destino,
               (struct sockaddr *)&endereco_destino,
               sizeof(struct sockaddr)) == -1) {
                perror("\n P2P:> erro: conectando no servidor\n");
                exit(1);
            }
            
            printf("\n P2P:> Cliente enviando ping...");
            if (send(porta_destino, ping(ip_meu, ip_destino), 200, 0) == -1)
                perror("\n P2P:> erro: nao conseguiu mandar mensagem");
            
            if ((numbytes=recv(porta_destino, buffer, 254, 0)) == -1) {
                perror("\n P2P:> erro: recv no peer cliente\n");
                exit(1);
            }
            
            buffer[numbytes] = '\0';

            printf("\n P2P:> Cliente recebeu: %s", buffer);
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
        }
    }



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
    if(!strcmp(comando, "q")) //"quit"
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
