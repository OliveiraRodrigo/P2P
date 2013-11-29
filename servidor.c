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
	struct sockaddr_in meu_endereco;
	struct sockaddr_in seu_endereco;
        char * meu_ip = (char*) malloc(20*sizeof(char));
        char * seu_ip = (char*) malloc(20*sizeof(char));
        char buffer[255];
	
	/* cria socket. PF_INET define IPv4, SOCK_STREAM define TCP */
	porta = socket(PF_INET, SOCK_STREAM, 0);
	
	/* verifica se foi criado */
	if (porta == -1){
		perror("erro: porta nao foi criada corretamente");
		exit(1);	
	}
	
	
	/* porta criada, agora faz o bind com o numero da porta desejado */
	meu_endereco.sin_family = AF_INET;
	meu_endereco.sin_port = htons(PORTA_SERVIDOR);
	meu_endereco.sin_addr.s_addr = INADDR_ANY;
        meu_ip = inet_ntoa(meu_endereco.sin_addr);
	 
	memset(&(meu_endereco.sin_zero), '\0', 8);
	
	
	if (bind(porta,(struct sockaddr *) &meu_endereco, sizeof(struct sockaddr_in)) == -1){
		perror("erro: nao conseguiu fazer bind\n");	
	}
	
	/* agora faz uma chamada ao listen*/
	if (listen(porta,50)==-1){
		perror("erro: problemas com o listen\n");
		exit(1);
	}
	
	/* agora verifica se ha conexoes */
	printf("\n Servidor Peer on-line, aguardando conexões.\n");
        
	while (1){
		tamanho = sizeof(struct sockaddr_in);
		
                /*Fica esperando aqui*/
                nova_porta = accept(porta, (struct sockaddr*)&seu_endereco, &tamanho);
                
                seu_ip = inet_ntoa(seu_endereco.sin_addr);
		
		if (nova_porta==-1){
			perror("erro: accept retornou erro\n");
			exit(1);
		}
                
                /*Agora testa se foi um ping que foi recebido.*/
                if ((numbytes=recv(nova_porta, buffer, 254, 0)) == -1) {
                        perror("erro: recv no peer servidor.\n");
                        exit(1);
                }
        	buffer[numbytes] = '\0';
                printf("\n Servidor recebeu: %s", buffer);
                if(!strcmp(buffer, ping(seu_ip, meu_ip))){
		    /* trata a conexao: simplesmente envia uma mensagem de volta */
                    printf("\n Servidor enviando pong...");
		
		    if (fork()==0){ /* se for o filho */
			close(porta); /* o filho nao aceita conexoes a mais */
			
			/*if (send(nova_porta, "Hello, world!\n", 14, 0) == -1)*/
                        if (send(nova_porta, pong(meu_ip, seu_ip), 200, 0) == -1)
			    perror("erro: nao conseguiu mandar mensagem");
			
			close(nova_porta);
			exit(0); /* tao logo termine, o filho pode sair */
                    }
		}
		//close(nova_porta); /* essa parte somente o pai executa */
	}
	
	//return 0; /* nunca chega aqui :-D */
}
/*
char * ping(char * meu_ip, char * seu_ip){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"ping\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", meu_ip, seu_ip);
    
    return saida;
}
*/
char * pong(char * meu_ip, char * seu_ip){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"pong\", "
                   "status:\"100\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", meu_ip, seu_ip);
    
    return saida;
}

char * agent_list(char * meu_ip, char * seu_ip){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"agent-list\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", meu_ip, seu_ip);
    
    return saida;
}

