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

void * servidor(){
    
    int porta, nova_porta, tamanho, numbytes;
    struct sockaddr_in endereco_meu;
    struct sockaddr_in endereco_cliente;
    char buffer[255];
    char * ip_meu     = (char*) malloc(20*sizeof(char));
    char * ip_cliente = (char*) malloc(20*sizeof(char));
    
    ip_meu = get_my_ip();
    //printf("\nS: meu ip: %s", ip_meu);
    
    /* cria socket. PF_INET define IPv4, SOCK_STREAM define TCP */
    porta = socket(PF_INET, SOCK_STREAM, 0);
    
    /* verifica se foi criado */
    if (porta == -1){
        perror("\n ::::: Erro: porta nao foi criada corretamente");
        exit(1);
    }
    
    /* porta criada, agora faz o bind com o numero da porta desejado */
    endereco_meu.sin_family = AF_INET;
    endereco_meu.sin_port = htons(PORTA_SERVIDOR);
    endereco_meu.sin_addr.s_addr = INADDR_ANY;
    memset(&(endereco_meu.sin_zero), '\0', 8);
    
    if (bind(porta,(struct sockaddr *) &endereco_meu, sizeof(struct sockaddr_in)) == -1){
        perror("\n ::::: Erro: servidor nao conseguiu fazer bind.\n");
    }
    
    /* agora faz uma chamada ao listen*/
    if (listen(porta,50)==-1){
        perror("\n ::::: Erro: servidor tem problemas com o listen\n");
        exit(1);
    }
    
    //printf("\n Servidor Peer on-line, aguardando conexões.\n");
    
    /* agora verifica se ha conexoes */
    while (1){
        tamanho = sizeof(struct sockaddr_in);
        
        /*Fica esperando aqui*/
        nova_porta = accept(porta, (struct sockaddr*)&endereco_cliente, &tamanho);
        
        ip_cliente = inet_ntoa(endereco_cliente.sin_addr);
        //printf("\nip_cliente: %s", ip_cliente);
        //printf("\nip_meu: %s", ip_meu);
        
        if (nova_porta==-1){
            perror("\n ::::: Erro: servidor: accept retornou erro\n");
            //exit(1);
            break;
        }
        
        if ((numbytes=recv(nova_porta, buffer, 254, 0)) == -1) {
            perror("\n ::::: Erro: servidor: recv no cliente.\n");
            //exit(1);
            break;
        }
        buffer[numbytes] = '\0';
        printf("\n ::::: Servidor recebeu: %s", buffer);
        
        /*Agora testa se foi um ping que foi recebido.*/
        //printf("\n ::::: Ping pra comparar: %s", ping(ip_cliente, ip_meu));
        if(!strcmp(buffer, ping(ip_cliente, ip_meu))){
            printf("\n ::::: Servidor enviando pong...");
            
            if (fork()==0){ /* se for o filho */
                close(porta); /* o filho nao aceita conexoes a mais */
                
                if (send(nova_porta, pong(ip_meu, ip_cliente), 200, 0) == -1){
                    perror("\n ::::: Erro: servidor nao conseguiu mandar mensagem");
                }
                
                close(nova_porta);
                exit(0); /* tao logo termine, o filho pode sair */
            }
        }
        else{
            send(nova_porta, "Erro no seu ping.", 200, 0);
        }
        //close(nova_porta); /* essa parte somente o pai executa */
    }
}

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

char * agent_list_back(int code, char ** ips, char * ip_meu, char * ip_destino){
    
    char * saida     = (char*) malloc(200 * sizeof(char));
    char * ips_list  = (char*) malloc(200 * sizeof(char));
    int i;
    
    i = 0;
    sprintf(ips_list, "%s", ips[i]);
    i++;
    ips++;
    while(**ips){
        sprintf(ips_list, "%s,%s", ips_list, ips[i]);
        i++;
        ips++;
    }
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"agent-list-back\", "
                   "status:\"%d\", "
                   "back:\"[%s\"], "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", code, ips_list, ip_meu, ip_destino);
    
    return saida;
}

char * authenticate_back(int code, char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"authenticate-back\", "
                   "status:\"%d\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", code, ip_meu, ip_destino);
    
    return saida;
}

char * archive_list_back(int code, archive_def * archs, char * ip_meu, char * ip_destino){
    
    char * saida       = (char*) malloc(200  * sizeof(char));
    char * archs_list  = (char*) malloc(2000 * sizeof(char));
    int i;
    
    i = 0;
    sprintf(archs_list, "%s:[", archs[i].type);
    sprintf(archs_list, "%sid:\"%d\", ", archs_list, archs[i].id);
    sprintf(archs_list, "%snome:\"%s\", ", archs_list, archs[i].name);
    sprintf(archs_list, "%ssize:\"%s\"]", archs_list, archs[i].size);
    i++;
    archs++;
////////////////////////////////////////////////////////////////////////////////
    while(archs){ // *archs
////////////////////////////////////////////////////////////////////////////////
        sprintf(archs_list, "%s, %s:[", archs_list, archs[i].type);
        sprintf(archs_list, "%sid:\"%d\", ", archs_list, archs[i].id);
        sprintf(archs_list, "%snome:\"%s\", ", archs_list, archs[i].name);
        sprintf(archs_list, "%ssize:\"%s\"]", archs_list, archs[i].size);
        i++;
        archs++;
    }
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"archive-list-back\", "
                   "status:\"%d\", "
                   "back:\"%s\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", code, archs_list, ip_meu, ip_destino);
    
    return saida;
}

char * archive_request_back(int code, archive_def arch, char * ip_meu, char * ip_destino){
    
    char * saida       = (char*) malloc(200  * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"archive-request-back\", "
                   "status:\"%d\", "
                   "id:\"%d\", "
                   
                   "http_address:\"no way, baby\", "
                /* "file:\"%s\" */
                   
                   "size:\"%s\", " //redundante
                   "md5:\"%s\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}",
                   code, arch.id,/*arch.file,*/ arch.size, arch.md5, ip_meu, ip_destino);
    
    return saida;
}
