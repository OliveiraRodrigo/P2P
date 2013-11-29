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

void * cliente(/*char * server*/){
	
        char * server_name = (char*) malloc(20*sizeof(char));
        int minha_porta, sua_porta, numbytes;
	struct sockaddr_in meu_endereco;
        struct sockaddr_in seu_endereco;
	struct hostent *he;
        char * meu_ip = (char*) malloc(20*sizeof(char));
        char * seu_ip = (char*) malloc(20*sizeof(char));
	
	char buffer[255];
        
/////////////////////////////////////////////////////////
	meu_endereco.sin_family = AF_INET;
	meu_endereco.sin_port = htons(PORTA_SERVIDOR);
	meu_endereco.sin_addr.s_addr = INADDR_ANY;
        meu_ip = inet_ntoa(meu_endereco.sin_addr);
	 
	memset(&(meu_endereco.sin_zero), '\0', 8);
/////////////////////////////////////////////////////////
        
	/* passe o nome do host onde esta o servidor e essa funcao retorna um endereco */
        server_name = "Rodrigo-PC";
        if ((he=gethostbyname(server_name)) == NULL) {
        /*if ((he=gethostbyaddr(seu_endereco.sin_addr.s_addr, 200, AF_INET)) == NULL) {*/
        //if ((he=getservent()) == NULL) {
		perror("erro: cliente nao conseguiu descobrir aonde esta o servidor\n");
		exit(1);
	}
        
        if ((sua_porta = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("erro: cliente nao conseguiu criar porta\n");
		exit(1);
	}
	
	/* prepara estrutura com endereco do servidor */
	seu_endereco.sin_family = AF_INET; 
	seu_endereco.sin_port = htons(PORTA_SERVIDOR);
	seu_endereco.sin_addr = *((struct in_addr *)he->h_addr);
	memset(&(seu_endereco.sin_zero), '\0', 8);
        //seu_ip = inet_ntoa(seu_endereco.sin_addr);
        seu_ip = "127.0.0.1";

	if (connect(sua_porta,
	   (struct sockaddr *)&seu_endereco,
	   sizeof(struct sockaddr)) == -1) {
		perror("erro: conectando no servidor\n");
		exit(1);
	}
        
        printf("\n Cliente enviando ping...");
        if (send(sua_porta, ping(meu_ip, seu_ip), 200, 0) == -1)
                perror("erro: nao conseguiu mandar mensagem");
        
	if ((numbytes=recv(sua_porta, buffer, 254, 0)) == -1) {
		perror("erro: recv no peer cliente\n");
		exit(1);
	}
	
	buffer[numbytes] = '\0';

	printf("\n Cliente recebeu: %s", buffer);

	while(1);
        //close(sua_porta);

	//return 0;

}

char * ping(char * meu_ip, char * seu_ip){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"ping\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", meu_ip, seu_ip);
    
    return saida;
}
