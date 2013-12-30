#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "comandos.h"
#define PORTA_SERVIDOR 9876
#define CHAVE "DiJqWHqKtiDgZySAv7ZX"

void * cliente(){
    
    int i, quant, caract, quit;
    int porta_destino, numbytes;
    char buffer[10000];
    char ips[50][20];
    char * comando[4];
    char ip_meu[20];
    char ip_destino[20];
    char ip_default[20];
    char truncName[80];
    protocolo protoin;
    archive_def * files;
    //double ti, tf, tempo; // ti = tempo inicial // tf = tempo final
    //struct timeval tempo_inicio, tempo_fim;
    
    /* Aloca espaco para 4 parametros com 50 caracteres cada */
    //comando = (char**) malloc(4*sizeof(char*));
    for(i = 0; i < 4; i++){
        comando[i] = (char*) calloc(50,sizeof(char));
    }
    
    /* Detecta o IP local e o armazena em 'ip_meu' */
    strcpy(ip_meu, get_my_ip());
    strcpy(ip_default, "\0");
    
/* Recebe comandos enquanto um deles nao for "quit" ***************************/
    quit = 0;
    while(!quit){
        
        /*ti = 0;
        tf = 0;
        tempo = 0;
        gettimeofday(&tempo_inicio, NULL);*/

        get_command(comando);
        if(!run_command(comando, ip_meu, ip_default, &quit)){
            
            if(!strcmp(comando[0], "try")){
                
                strcpy(ip_destino, set_ipdestino(comando[1], ip_default));
                if(strcmp(ip_destino, "\0")){
                    
                    porta_destino = porta(ip_destino);
                    if(porta_destino != -1){
                        
                        clear_line
                        green printf(" P2P:> ");
                        cyan printf("Enviando ping...\n");
                        //printf("\n P2P:> %s", ping(ip_meu, ip_destino));
                        
                        if(send(porta_destino, ping(ip_meu, ip_destino), 999, 0) != -1){
                            
                            if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                                
                                buffer[numbytes] = '\0';
                                //printf("\n P2P:> Cliente recebeu: %s", buffer);
                                protoin = set_proto(buffer);
                                
                                //Testa se recebeu um pong ok.
                                if(protoin.ok && !strcmp(protoin.command, "pong")){
                                    green printf(" P2P:> ");
                                    orange printf("%s", ip_destino);
                                    cyan printf(" respondeu corretamente.\n");
                                }
                                else{
                                    green printf(" P2P:> ");
                                    red printf("Erro: ");
                                    orange printf("%s", ip_destino);
                                    red printf(" retornou %s codigo ", protoin.command);
                                    orange printf("%d", protoin.status);
                                    red printf(".\n");
                                }
                            }
                            else{
                                green printf(" P2P:> ");
                                red printf("Erro: nao foi possivel receber 'pong'\n");
                            }
                        }
                        else{
                            green printf(" P2P:> ");
                            red printf("Erro: nao foi possivel enviar 'ping'\n");
                        }
                        close(porta_destino);
                    }
                }
            }
            else{
                if(!strcmp(comando[0], "login")){
                    
                    strcpy(ip_destino, set_ipdestino(comando[1], ip_default));
                    if(strcmp(ip_destino, "\0")){
                        
                        porta_destino = porta(ip_destino);
                        if(porta_destino != -1){
                        
                            clear_line
                            green printf(" P2P:> ");
                            cyan printf("Enviando authenticate...\n");
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
                                                green printf(" P2P:> ");
                                                cyan printf("Autenticacao com ");
                                                orange printf("%s", ip_destino);
                                                cyan printf(" aceita.\n");
                                                //Salva o IP para desconectar ao sair.
                                                insert_ip(CLIENT, ips, ip_destino); //Cliente nao precisa desta lista
                                            }
                                            else{
                                                green printf(" P2P:> ");
                                                red printf("Erro: autenticacao com ");
                                                orange printf("%s", ip_destino);
                                                red printf(" falhou. Codigo ");
                                                orange printf("%d", protoin.status);
                                                red printf(".\n");
                                            }
                                        }
                                        else{
                                            green printf(" P2P:> ");
                                            red printf("Erro: ");
                                            orange printf("%s", ip_destino);
                                            red printf(" retornou %s codigo ", protoin.command);
                                            orange printf("%d", protoin.status);
                                            red printf(".\n");
                                        }
                                    }
                                    else{
                                        green printf(" P2P:> ");
                                        red printf("Erro: ");
                                        orange printf("%s", ip_destino);
                                        red printf(" retornou um protocolo incompativel.\n");
                                    }
                                }
                                else{
                                    green printf(" P2P:> ");
                                    red printf("Erro: nao foi possivel receber 'authenticate-back'\n");
                                }
                            }
                            else{
                                green printf(" P2P:> ");
                                red printf("Erro: nao foi possivel enviar 'authenticate'\n");
                            }
                            close(porta_destino);
                        }
                    }
                }
                else{
                    if(!strcmp(comando[0], "list-users")){
                        
                        strcpy(ip_destino, set_ipdestino(comando[1], ip_default));
                        if(strcmp(ip_destino, "\0")){
                            
                            porta_destino = porta(ip_destino);
                            if(porta_destino != -1){
                                
                                clear_line
                                green printf(" P2P:> ");
                                cyan printf("Enviando agent-list...\n");
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
                                                    green printf(" P2P:> ");
                                                    cyan printf("IPs: ");
                                                    orange printf("%s\n", protoin.back);
                                                }
                                                else{
                                                    green printf(" P2P:> ");
                                                    red printf("Erro: ");
                                                    orange printf("%s", ip_destino);
                                                    red printf(" informou o codigo ");
                                                    orange printf("%d", protoin.status);
                                                    red printf(".\n");
                                                }
                                            }
                                            else{
                                                green printf(" P2P:> ");
                                                red printf("Erro: ");
                                                orange printf("%s", ip_destino);
                                                red printf(" retornou %s codigo ", protoin.command);
                                                orange printf("%d", protoin.status);
                                                red printf(".\n");
                                            }
                                        }
                                        else{
                                            green printf(" P2P:> ");
                                            red printf("Erro: ");
                                            orange printf("%s", ip_destino);
                                            red printf(" retornou um protocolo incompativel.\n");
                                        }
                                    }
                                    else{
                                        green printf(" P2P:> ");
                                        red printf("Erro: nao foi possivel receber 'agent-list-back'\n");
                                    }
                                }
                                else{
                                    green printf(" P2P:> ");
                                    red printf("Erro: nao foi possivel enviar 'agent-list'\n");
                                }
                                close(porta_destino);
                            }
                        }
                    }
                    else{
                        if(!strcmp(comando[0], "list-files")){
                            
                            strcpy(ip_destino, set_ipdestino(comando[1], ip_default));
                            if(strcmp(ip_destino, "\0")){
                                
                                porta_destino = porta(ip_destino);
                                if(porta_destino != -1){
                                    
                                    clear_line
                                    green printf(" P2P:> ");
                                    cyan printf("Enviando archive-list...\n");
                                    //printf("\n P2P:> %s", archive_list(ip_meu, ip_destino));
                                    
                                    if(send(porta_destino, archive_list(ip_meu, ip_destino), 999,0) != -1){
                                        
                                        if((numbytes = recv(porta_destino, buffer, 9999, 0)) != -1) {
                                            
                                            buffer[numbytes] = '\0';
                                            //printf("\n P2P:> Cliente recebeu: %s", buffer);
                                            protoin = set_proto(buffer);
                                            
                                            //Testa se recebeu um archive-list-back ok.
                                            if(protoin.ok){
                                                if(!strcmp(protoin.command, "archive-list-back")){
                                                    if(protoin.status == 200){
                                                        files = malloc(1000*sizeof(archive_def));
                                                        quant = getFileList(protoin.back, files);
                                                        green printf(" P2P:> ");
                                                        cyan printf("Arquivos de ");
                                                        orange printf("%s", ip_destino);
                                                        cyan printf(":\n\n");
                                                        //orange printf("\"%s\"\n", protoin.back);
                                                        printf("%7s", " ");
                                                        bold under green printf("%s%6s%56s\n","ID","NAME","SIZE");
                                                        reset
                                                        i = 0;
                                                        while(i < quant){
                                                            green printf("%9d", files[i].id);
                                                            strcpy(truncName, "\0");
                                                            if(strlen(files[i].name) > 59-strlen(files[i].size)){
                                                                strncat(truncName, files[i].name, 56-strlen(files[i].size));
                                                                strcat(truncName, "...");
                                                            }
                                                            else{
                                                                strcpy(truncName, files[i].name);
                                                            }
                                                            orange caract = printf("  %s", truncName);
                                                            red printf("%*s\n", 62-caract, files[i].size);
                                                            i++;
                                                        }
                                                    }
                                                    else{
                                                        green printf(" P2P:> ");
                                                        red printf("Erro: ");
                                                        orange printf("%s", ip_destino);
                                                        red printf(" informou o codigo ");
                                                        orange printf("%d", protoin.status);
                                                        red printf(".\n");
                                                    }
                                                }
                                                else{
                                                    green printf(" P2P:> ");
                                                    red printf("Erro: ");
                                                    orange printf("%s", ip_destino);
                                                    red printf(" retornou %s codigo ", protoin.command);
                                                    orange printf("%d", protoin.status);
                                                    red printf(".\n");
                                                }
                                            }
                                            else{
                                                green printf(" P2P:> ");
                                                red printf("Erro: ");
                                                orange printf("%s", ip_destino);
                                                red printf(" retornou um protocolo incompativel.\n");
                                            }
                                        }
                                        else{
                                            green printf(" P2P:> ");
                                            red printf("Erro: nao foi possivel receber 'archive-list-back'\n");
                                        }
                                    }
                                    else{
                                        green printf(" P2P:> ");
                                        red printf("Erro: nao foi possivel enviar 'archive-list'\n");
                                    }
                                    close(porta_destino);
                                }
                            }
                        }
                        else{
                            if(!strcmp(comando[0], "down")){
                                
                                strcpy(ip_destino, set_ipdestino(comando[2], ip_default));
                                if(strcmp(ip_destino, "\0")){
                                    
                                    porta_destino = porta(ip_destino);
                                    if(porta_destino != -1){
                                        
                                        clear_line
                                        green printf(" P2P:> ");
                                        cyan printf("Enviando archive-request...\n");
                                        //printf("\n P2P:> %s", archive_request(comando[1], ip_meu, ip_destino));
                                        
                                        if(send(porta_destino, archive_request(comando[1], ip_meu, ip_destino), 999,0) != -1){
                                            
                                            if((numbytes = recv(porta_destino, buffer, 999, 0)) != -1) {
                                                
                                                buffer[numbytes] = '\0';
                                                //printf("\n P2P:> Cliente recebeu: %s", buffer);
                                                protoin = set_proto(buffer);
                                                
                                                //Testa se recebeu um archive-request-back ok.
                                                if(protoin.ok){
                                                    if(!strcmp(protoin.command, "archive-request-back")){
                                                        if(protoin.status == 302){
                                                            green printf(" P2P:> ");
                                                            orange printf("%s", ip_destino);
                                                            cyan printf(" enviou os dados do arquivo ");
                                                            orange printf("%d", protoin.file.id);
                                                            cyan printf(":\n\n\t Link: ");
                                                            orange printf("%s", protoin.file.http);
                                                            cyan printf("\n\t MD5:  ");
                                                            orange printf("%s\n", protoin.file.md5);
                                                            //baixaArquivo(ip_destino, 8000, protoin.file.http);
                                                        }
                                                        else{
                                                            green printf(" P2P:> ");
                                                            red printf("Erro: ");
                                                            orange printf("%s", ip_destino);
                                                            red printf(" informou o codigo ");
                                                            orange printf("%d", protoin.status);
                                                            red printf(".\n");
                                                        }
                                                    }
                                                    else{
                                                        green printf(" P2P:> ");
                                                        red printf("Erro: ");
                                                        orange printf("%s", ip_destino);
                                                        red printf(" retornou %s codigo ", protoin.command);
                                                        orange printf("%d", protoin.status);
                                                        red printf(".\n");
                                                    }
                                                }
                                                else{
                                                    green printf(" P2P:> ");
                                                    red printf("Erro: ");
                                                    orange printf("%s", ip_destino);
                                                    red printf(" retornou um protocolo incompativel.\n");
                                                }
                                            }
                                            else{
                                                green printf(" P2P:> ");
                                                red printf("Erro: nao foi possivel receber 'archive-request-back'\n");
                                            }
                                        }
                                        else{
                                            green printf(" P2P:> ");
                                            red printf("Erro: nao foi possivel enviar 'archive-request'\n");
                                        }
                                        close(porta_destino);
                                    }
                                }
                            }
                            else{
                                if(!strcmp(comando[0], "logout")){
                                    
                                    strcpy(ip_destino, set_ipdestino(comando[1], ip_default));
                                    if(strcmp(ip_destino, "\0")){
                                        
                                        porta_destino = porta(ip_destino);
                                        if(porta_destino != -1){
                                            
                                            clear_line
                                            green printf(" P2P:> ");
                                            cyan printf("Enviando end-connection...\n");
                                            //printf("\n P2P:> %s", end-connection(ip_meu, ip_destino));
                                            
                                            if(send(porta_destino, end_connection(ip_meu, ip_destino), 999,0) != -1){
                                                green printf(" P2P:> ");
                                                cyan printf("Desconectado de ");
                                                orange printf("%s", ip_destino);
                                                cyan printf(".\n");
                                            }
                                            else{
                                                green printf(" P2P:> ");
                                                red printf("Erro: nao foi posivel enviar 'end-connection'\n");
                                            }
                                            //Remove o IP da lista, pois, ao sair, deste eu ja estou desconectado.
                                            remove_ip(CLIENT, ips, ip_destino); //Cliente nao precisa desta lista
                                            close(porta_destino);
                                        }
                                    }
                                }
                                else{
                                    clear_line
                                    green printf(" P2P:> ");
                                    red printf("Comando inexistente: ");
                                    orange printf("%s\n", comando[0]);
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
        /*gettimeofday(&tempo_fim,NULL);
        tf = (double)tempo_fim.tv_usec + ((double)tempo_fim.tv_sec * (1000000.0));
        ti = (double)tempo_inicio.tv_usec + ((double)tempo_inicio.tv_sec * (1000000.0));
        tempo = (tf - ti) / 1000;
        printf("Tempo: %.3f\n",tempo);*/
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
                        clear_line
                        green printf("\n P2P:> ");
                        red printf("Erro: Nao foi possivel localizar ");
                        orange printf("%s", ip_destino);
                        red printf(".\n");
                        //addr_destino = temp_addr;
                        return -1;
                    }
                    
                    if((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                        clear_line
                        red printf("\n P2P:> Erro: Nao foi possivel criar a porta\n");
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
                        clear_line
                        red printf("\n P2P:> Erro: conectando no servidor\n");
                        return -1;
                    }
                    return porta_destino;
}

int baixaArquivo(char shost[64], int porta, char url[128])
{
    char req[512]; //requisição
    int sock; //socket
    struct sockaddr_in addr;
    struct hostent  * host = NULL;
    
    char buffer[512]; // Buffer para armazenar dados

    int bytes = 1; // Bytes recebidos
    int flagAcabouCabecalho=0;
    int cont=0;
    char *pSplit=NULL;
    FILE *fp; //arquivo para escrever dados baixados
    char nomeArquivo[128];
    
    pSplit = (char*) malloc(100*sizeof(char));
    
    sprintf(nomeArquivo, "%s.part", url);
    fp=fopen(nomeArquivo,"w+b"); //abra arquivo
    if(fp==NULL){
       printf("erro ao criar arquivo");
       return 1;
    }

    
    // Cria socket para conectar ao servidor http
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0){
        printf("ERRO ao criar socket[1]");
        return 1;
    }
    addr.sin_family=AF_INET;
    addr.sin_port = htons(porta);//seta porta
    host = gethostbyname(shost);//sera host

    if(host == NULL){ //se host não responde, cai fora
        printf("ERRO ao criar socket [2]");
        return 1;
    }
    
    //ajusta ip do host
    memcpy(&addr.sin_addr,host->h_addr_list[0],host->h_length);

    //conecta
    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0){
        printf("ERRO ao conectar ao servidor para download");
        return 1;
    }

    // Requisição
    sprintf(req,"GET /%s HTTP/1.1\r\n\r\n",url);//monta requisição com a url
    
    send(sock,req,strlen(req),0); // Envia requisição


    while(bytes > 0) // Enquanto estiver recebendo
    {        
        memset(buffer,0,512); // Limpa o buffer
        bytes = recv(sock,buffer,512,0); // Recebe dados
       
        if(bytes>0){
            cont++;
            printf("\nRecebendo parte %d do arquivo...\n",cont);
        }
        
        if(flagAcabouCabecalho==0){ //se o cabeçalho ainda não acabou, busca pelo fim dele
            printf("--aqui--");
            pSplit = strstr(buffer, "\r\n\r\n"); //se achar o fim do cabeçalho, retorna ponteiro apontando para ele
            printf("--aqui 0--");
            pSplit+=4;
            //printf("\n1\n");
            printf("--aqui 0.5--");
        }
        else{
            pSplit=NULL;
            printf("\n2\n");
        }
        if(pSplit != NULL) {
            printf("--aqui 1--");
            flagAcabouCabecalho=1;//se cabeçalho acabou, seta a flag
            printf("%s",pSplit); //imprime parte do buffer que está após o cabeçalho
            fprintf(fp,"%s",pSplit); //salva parte do buffer que está após o cabeçalho
            printf("--aqui 2--");
            printf("\n3\n");
        }
        else{
            printf("--aqui 3--");
            if(flagAcabouCabecalho==1){ //se cabeçalho já acabou, imprime buffer inteiro
                printf("--aqui 4--");
                printf("%s",buffer);
                fprintf(fp,"%s",buffer);
                printf("\n4\n");
            }
            printf("--aqui 5--");
        }
        printf("--aqui 6--");
    }
    printf("--aqui 7--");
    
    sprintf(req, "mv %s %s", nomeArquivo, url);//monta string pro system a seguir
    system(req); //renomeia o arquivo para o nome original (sem ".part" que havia sido adicionado)
    
    printf("\nDownload concluído!\n");
    
    close(sock); // Fecha socket
    fclose(fp);
    
    return 0;
}
/*
Sistema de Consulta Alunos - DRA_UFPel - CÃ³pia.pdf                  
Sistema de Consulta Alunos - DRA_UFPel - CopiaSistema de Consulta Alunos - DRA_UFPel - Copia.pdf
 */