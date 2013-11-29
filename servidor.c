#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define PORTA_SERVIDOR 9876

void * servidor(){

	int porta;
	int nova_porta;
	int tamanho;
        int numbytes;
	struct sockaddr_in endereco_meu;
	struct sockaddr_in endereco_destino;
        char * ip_meu = (char*) malloc(20*sizeof(char));
        char * ip_destino = (char*) malloc(20*sizeof(char));
        char buffer[255];
	
	/* cria socket. PF_INET define IPv4, SOCK_STREAM define TCP */
	porta = socket(PF_INET, SOCK_STREAM, 0);
	
	/* verifica se foi criado */
	if (porta == -1){
		perror("\n ::::: erro: porta nao foi criada corretamente");
		exit(1);	
	}
	
	
	/* porta criada, agora faz o bind com o numero da porta desejado */
	endereco_meu.sin_family = AF_INET;
	endereco_meu.sin_port = htons(PORTA_SERVIDOR);
	endereco_meu.sin_addr.s_addr = INADDR_ANY;
        ip_meu = inet_ntoa(endereco_meu.sin_addr);
	 
	memset(&(endereco_meu.sin_zero), '\0', 8);
	
	
	if (bind(porta,(struct sockaddr *) &endereco_meu, sizeof(struct sockaddr_in)) == -1){
		perror("\n ::::: erro: servidor nao conseguiu fazer bind\n");	
	}
	
	/* agora faz uma chamada ao listen*/
	if (listen(porta,50)==-1){
		perror("\n ::::: erro: servidor tem problemas com o listen\n");
		exit(1);
	}
	
	/* agora verifica se ha conexoes */
	//printf("\n Servidor Peer on-line, aguardando conexões.\n");
        
	while (1){
		tamanho = sizeof(struct sockaddr_in);
		
                /*Fica esperando aqui*/
                nova_porta = accept(porta, (struct sockaddr*)&endereco_destino, &tamanho);
                
                ip_destino = inet_ntoa(endereco_destino.sin_addr);
		
		if (nova_porta==-1){
			perror("\n ::::: erro: servidor: accept retornou erro\n");
			exit(1);
		}
                
                /*Agora testa se foi um ping que foi recebido.*/
                if ((numbytes=recv(nova_porta, buffer, 254, 0)) == -1) {
                        perror("\n ::::: erro: servidor: recv no cliente.\n");
                        exit(1);
                }
        	buffer[numbytes] = '\0';
                printf("\n ::::: Servidor recebeu: %s", buffer);
                if(!strcmp(buffer, ping(ip_destino, ip_meu))){
		    /* trata a conexao: simplesmente envia uma mensagem de volta */
                    printf("\n ::::: Servidor enviando pong...");
		
		    if (fork()==0){ /* se for o filho */
			close(porta); /* o filho nao aceita conexoes a mais */
			
			/*if (send(nova_porta, "Hello, world!\n", 14, 0) == -1)*/
                        if (send(nova_porta, pong(ip_meu, ip_destino), 200, 0) == -1)
			    perror("\n ::::: erro: servidor nao conseguiu mandar mensagem");
			
			close(nova_porta);
			exit(0); /* tao logo termine, o filho pode sair */
                    }
		}
		//close(nova_porta); /* essa parte somente o pai executa */
	}
	
	//return 0; /* nunca chega aqui :-D */
}
/*
char * ping(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"ping\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}
*/
char * pong(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"pong\", "
                   "status:\"100\", "
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

