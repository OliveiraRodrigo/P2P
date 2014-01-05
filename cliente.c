#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <resolv.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "comandos.h"

#define CHAVE "DiJqWHqKtiDgZySAv7ZX"
#define USERAGENT "HTMLGET 1.0"

void * cliente(){
    
    int i, quant, caract, quit, numbytes;
    intptr_t porta_destino;
    char buffer[10000];
    char * comando[4];
    char ip_meu[20];
    char ip_destino[20];
    char ip_default[20];
    char truncName[80];
    float formSize;
    protocolo protoin;
    archive_def * files;
    IPs ips;
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
                    
                    porta_destino = porta(ip_destino, PORTA_SERVIDOR);
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
                        
                        porta_destino = porta(ip_destino, PORTA_SERVIDOR);
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
                                                //insert_ip(CLIENT, ips, ip_destino);
                                                ips_list(INSERT, CLIENT, ip_destino, NULL);
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
                            
                            porta_destino = porta(ip_destino, PORTA_SERVIDOR);
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
                                
                                porta_destino = porta(ip_destino, PORTA_SERVIDOR);
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
                                                        bold green printf("%s%6s%56s","ID","NAME","SIZE");
                                                        printf("\n ----------------------------------------------------------------------\n");
                                                        reset
                                                        i = 0;
                                                        while(i < quant){
                                                            green printf("%9d", files[i].id);
                                                            strcpy(truncName, "\0");
                                                            if(strlen(files[i].name) > 56-strlen(files[i].size)){
                                                                strncat(truncName, files[i].name, 53-strlen(files[i].size));
                                                                strcat(truncName, "...");
                                                            }
                                                            else{
                                                                strcpy(truncName, files[i].name);
                                                            }
                                                            orange caract = printf("  %s", truncName);
                                                            formSize = atof(files[i].size);
                                                            if(formSize >= 1024){
                                                                formSize = atof(files[i].size)/1024;
                                                                red printf("%*.2f MB\n", 59-caract, formSize);
                                                            }
                                                            else{
                                                                red printf("%*.2f KB\n", 59-caract, formSize);
                                                            }
                                                            i++;
                                                        }
                                                        bold green printf(" ----------------------------------------------------------------------\n");
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
                                    
                                    porta_destino = porta(ip_destino, PORTA_SERVIDOR);
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
                                                            //baixaArquivo(ip_destino/*, PORTA_HTTP*/, protoin.file.http);
                                                            //httpClient(ip_destino/*, PORTA_HTTP*/, protoin.file.http);
                                                            down(ip_destino, protoin.file.http);
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
                                        
                                        porta_destino = porta(ip_destino, PORTA_SERVIDOR);
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
                                            //remove_ip(CLIENT, ips, ip_destino);
                                            ips_list(REMOVE, CLIENT, ip_destino, NULL);
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
            /*for(i = 0; i < client_ips_size(0); i++){
                send(porta_destino, end_connection(ip_meu, ips[i]), 999,0);
            }*/
            ips_list(GET, CLIENT, NULL, ips);
            for(i = 0; i < ips_list(GETSIZE, CLIENT, NULL, NULL); i++){
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

intptr_t porta(char * ip_destino, intptr_t porta_remota){
	
    int numbytes, codigo;
    intptr_t porta_destino;
    struct sockaddr_in endereco_destino;
    struct hostent *he;
    long addr_destino, temp_addr;
	
                    addr_destino = inet_addr(ip_destino);
                    
                    if((he=gethostbyaddr((char *) &addr_destino, sizeof(addr_destino), AF_INET)) == NULL) {
                        clear_line
                        green printf("\n P2P:> ");
                        red printf("Erro: Nao foi possivel localizar ");
                        orange printf("%s", ip_destino);
                        red printf(".\n");
                        return -1;
                    }
                    
                    if((porta_destino = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                        clear_line
                        red printf("\n P2P:> Erro: Nao foi possivel criar a porta\n");
                        return -1;
                    }
                    
                    // prepara estrutura com endereco do servidor
                    endereco_destino.sin_family = AF_INET; 
                    endereco_destino.sin_port = htons(porta_remota);
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

int baixaArquivo(char ip_destino[20]/*, int porta*/, char url[128]){
    
    char req[512], shost[64];
    intptr_t sock; //socket
    struct sockaddr_in addr;
    struct in_addr IPV4 = { 0 };
    //long IPV4;
    struct hostent * host = NULL;
    char buffer[10000]; // Buffer para armazenar dados
    int bytes = 1; // Bytes recebidos
    int flagAcabouCabecalho = 0;
    int cont = 0;
    char *pSplit = NULL;
    FILE *fp; //arquivo para escrever dados baixados
    char nomeArquivo[128];
    
    pSplit = (char*) malloc(10000*sizeof(char));
    
    sprintf(nomeArquivo, "%s.part", url);
    fp=fopen(nomeArquivo,"wb"); //abra arquivo
    if(fp==NULL){
       printf("erro ao criar arquivo");
       return 1;
    }

    /*
    // Cria socket para conectar ao servidor http
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0){
        printf("ERRO ao criar socket[1]");
        return 1;
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(porta);//seta porta
    IPV4.s_addr = inet_addr(ip_destino);
    //inet_aton(ip_destino, &IPV4);
    host = gethostbyaddr((char *)&IPV4, sizeof(IPV4), AF_INET);
    
    if(host == NULL){ //se host não responde, cai fora
        printf("ERRO ao criar socket [2]");
        return 1;
    }
    
    //ajusta ip do host
    memcpy(&addr.sin_addr,host->h_addr_list[0],host->h_length);

    //conecta
    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0){
        printf(" P2P:> ERRO ao conectar ao servidor para download\n");
        return 1;
    }
    */
    sock = porta(ip_destino, PORTA_HTTP);
    
    // Requisição
    sprintf(req,"GET /%s HTTP/1.1\r\n\r\n",url);//monta requisição com a url
    
    send(sock,req,strlen(req),0); // Envia requisição


    while(bytes > 0){ // Enquanto estiver recebendo
        
        memset(buffer,0,10000); // Limpa o buffer
        bytes = recv(sock,buffer,9999,0); // Recebe dados
        
        if(bytes > 0){
            cont++;
            printf("\nRecebendo parte %d do arquivo...\n", cont);
        }
        
        if(!flagAcabouCabecalho){ //se o cabeçalho ainda não acabou, busca pelo fim dele
            printf("--aqui--");
            pSplit = strstr(buffer, "\r\n\r\n"); //se achar o fim do cabeçalho, retorna ponteiro apontando para ele
            printf("--aqui 0--");
            pSplit+=4;
            //printf("\n1\n");
            printf("--aqui 0.5--");
        }
        else{
            printf("\n2\n");
            pSplit = NULL;
        }
        if(pSplit != NULL) {
            printf("--aqui 1--");
            flagAcabouCabecalho=1;//se cabeçalho acabou, seta a flag
            //printf("%s",pSplit); //imprime parte do buffer que está após o cabeçalho
            fprintf(fp,"%s",pSplit); //salva parte do buffer que está após o cabeçalho
            printf("--aqui 2--");
            printf("\n3\n");
        }
        else{
            printf("--aqui 3--");
            if(flagAcabouCabecalho==1){ //se cabeçalho já acabou, imprime buffer inteiro
                printf("--aqui 4--");
                //printf("%s",buffer);
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

int httpClient(char ip[20]/*, int porta*/, char url[128]){
    
  struct sockaddr_in *remote;
  struct in_addr IPV4 = { 0 };
  struct hostent * host = NULL;
  intptr_t sock;
  int tmpres;
  char *get;
  char buf[BUFSIZ+1];
  //char *host;
  FILE *fp; //arquivo para escrever dados baixados
  char nomeArquivo[128];
  char req[512];
 
  /*//sock = create_tcp_socket();
  if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
    perror("Can't create TCP socket");
    exit(1);
  }
  fprintf(stderr, "IP is %s\n", ip);
  remote = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in *));
  remote->sin_family = AF_INET;
  tmpres = inet_pton(AF_INET, ip, (void *)(&(remote->sin_addr.s_addr)));
  
  IPV4.s_addr = inet_addr(ip);
  host = gethostbyaddr((char *)&IPV4, sizeof(IPV4), AF_INET);
  
  if( tmpres < 0)  
  {
    perror("Can't set remote->sin_addr.s_addr");
    exit(1);
  }else if(tmpres == 0)
  {
    fprintf(stderr, "%s is not a valid IP address\n", ip);
    exit(1);
  }
  remote->sin_port = htons(porta);
 
  if(connect(sock, (struct sockaddr *)remote, sizeof(struct sockaddr)) < 0){
    perror("Could not connect");
    exit(1);
  }*/
  
  get = (char*) malloc(10000*sizeof(char));
  
  sprintf(nomeArquivo, "%s.part", url);
    fp=fopen(nomeArquivo,"wb"); //abra arquivo
    if(fp==NULL){
       printf("erro ao criar arquivo");
       return 1;
    }
    
  /*IPV4.s_addr = inet_addr(ip);
  host = gethostbyaddr((char *)&IPV4, sizeof(IPV4), AF_INET);*/
  
  sock = porta(ip, PORTA_HTTP);
  //sock = porta2(/*ip*/"200.132.39.116", /*PORTA_HTTP*/80);
  
  //get = build_get_query(host->h_name, url);
  //http://www.cs.utah.edu/~swalton/Documents/Articles/Writing-a-C-Based-Client-Server.pdf
  //http://www.ufsm.br/ccr/revista/boleto.pdf
  sprintf(get,"GET /%s HTTP/1.1\r\n\r\n",url);//monta requisição com a url
  fprintf(stderr, "Query is:\n<<START>>\n%s<<END>>\n", get);
 
  //Send the query to the server
  int sent = 0;
  while(sent < strlen(get))
  {
    tmpres = send(sock, get+sent, strlen(get)-sent, 0);
    if(tmpres == -1){
      perror("Can't send query");
      exit(1);
    }
    sent += tmpres;
  }
  //now it is time to receive the page
  memset(buf, 0, sizeof(buf));
  int htmlstart = 0;
  char * htmlcontent;
  htmlcontent = (char*) malloc(1024*sizeof(char));
  int i = 0;
  printf("-->");
  while((tmpres = recv(sock, buf, BUFSIZ, 0)) > 0){
printf("[0]");
printf("%s", buf);
      if(!htmlstart){
//printf("[1]");
     /* Under certain conditions this will not work.
      * If the \r\n\r\n part is splitted into two messages
      * it will fail to detect the beginning of HTML content
      */
      htmlcontent = strstr(buf, "\r\n\r\n");
      if(htmlcontent != NULL){
//printf("[2]");
        htmlstart = 1;
        htmlcontent += 4;
      }
    }
    else{
//printf("[3]");
      htmlcontent = buf;
    }
    if(htmlstart){
//printf("[4]");
      //printf("\nsim\n");
      fprintf(fp,"%s", htmlcontent);
        //printf("\n%d ",strlen(htmlcontent));
        //printf("[%d]\n",sizeof(htmlcontent));
        //fprintf(stdout, htmlcontent);
    }
    else{
//printf("[5]",i++);
        //printf("\nnao\n");
    }
 
    //memset(buf, 0, tmpres);
//printf("[6]",i++);
  }
printf("<--");
  if(tmpres < 0)
  {
    perror("Error receiving data");
  }
  /*free(get);
  free(remote);
  free(ip);*/
  
    sprintf(req, "mv %s %s", nomeArquivo, url);//monta string pro system a seguir
    system(req); //renomeia o arquivo para o nome original (sem ".part" que havia sido adicionado)
    
    printf("\nDownload concluído!\n");
  
  close(sock);
  fclose(fp);
  return 0;
}
 
char *build_get_query(const char *host, char *page)
{
  char *query;
  char *getpage = page;
  char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
  if(getpage[0] == '/'){
    getpage = getpage + 1;
    fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", page, getpage);
  }
  // -5 is to consider the %s %s %s in tpl and the ending \0
  query = (char *)malloc(strlen(host)+strlen(getpage)+strlen(USERAGENT)+strlen(tpl)-5);
  sprintf(query, tpl, getpage, host, USERAGENT);
  return query;
}

int down(char ip[20], char url[128]){
    
    struct hostent* host;
    struct in_addr IPV4 = { 0 };
    /*host = gethostbyname("lwn.net");
    printf("IP address = %s\n", 
    inet_ntoa(*(long*)host->h_addr_list[0]));struct servent *srv;
    srv = getservbyname("http", "tcp");
    printf("%s: port=%d\n", srv->s_name, ntohs(srv->s_port));*/
    
    intptr_t sd;
    //sd = socket(PF_INET, SOCK_STREAM, 0); /* create socket */
    sd = porta(ip, PORTA_HTTP);
    
    IPV4.s_addr = inet_addr(ip);
    host = gethostbyaddr((char *)&IPV4, sizeof(IPV4), AF_INET);
    //printf("\n{%s}\n", host->h_name);
    
    //struct sockaddr_in addr;
    //memset(&addr, 0, sizeof(addr));    /* create & zero struct */
    //addr.sin_family = AF_INET;    /* select internet protocol */
    //addr.sin_port = srv->s_port;         /* set the port # */
    //addr.sin_addr.s_addr = *(long*)host->h_addr_list[0]; /* set the addr */
    //connect(sd, &addr, sizeof(addr));         /* connect! */
    
    char req[200], s[BUFSIZ+1], header[BUFSIZ+1], content[BUFSIZ+1], code[4];
    FILE *stream, *file;
    
    //int htmlstart = 0;
    int c, h, i, isHeader;
    //char * htmlcontent;
    //htmlcontent = (char*) malloc(10000*sizeof(char));
    memset(req, 0, sizeof(req));
    memset(s, 0, sizeof(s));
    memset(header, 0, sizeof(header));
    memset(content, 0, sizeof(content));
    memset(code, 0, sizeof(code));
    
    stream = fdopen(sd, "r+");         /* convert into stream */
    file = fopen(url,"wb"); //abra arquivo
    
    //sprintf(req, "GET /%s HTTP/1.0\r\n\r\n", "/ccr/revista/boleto.pdf"/*url*/);
    sprintf(req, "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n",
                 /*"/ccr/revista/boleto.pdf"*/url,
                 host->h_name, USERAGENT);
    
    fprintf(stream, req);      // send request
    fflush(stream);            // ensure it got out
    
    //i = 0;
    h = 0;
    isHeader = 1;
    while ( fgets(s, sizeof(s), stream) != 0 ){  /* while not EOF ...*/
        //printf("[0]");
        //printf("%s", s);
        /*if(!htmlstart){
//printf("[1]");
            htmlcontent = strstr(s, "\r\n\r\n");//ta chegando so "\r\n"
//          while(i < sizeof(s)-4){
//printf("[%d]", s[i]);
//              if(s[i]==13 && s[i+1]==10){
//printf("%s", htmlcontent);
                    if(htmlcontent != NULL){
//printf("[2]");
                        htmlstart = 1;
                        htmlcontent += i+4;
                        //break;
                    }
                /*}
                else{
                    i++;
                }*//*
          //}
        }
        else{
//printf("[3]");
            htmlcontent = s;
        }
        if(htmlstart){
//printf("[4]");
            fputs(htmlcontent, stdout);           //... print the data
            //fprintf(fp,"%s", htmlcontent);
        }
//printf("[5]");*/
        //fputs(s, stdout);           //... print the data
        
        //fputs(s, file);
        i = 0;
        while(i < strlen(s)){
            if(isHeader){
                header[h] = s[i];
                h++;
                header[h] = '\0';
                if(strstr(header, "\r\n\r\n") != NULL){
                    isHeader = 0;
                    c = 0;
                    for(h = 9; h < 12; h++){
                        code[c] = header[h];
                        c++;
                    }
                    code[c] = '\0';
                    if(strcmp(code, "200")){
                        return atoi(code);
                    }
                }
            }
            else{
                sprintf(content, "%c", s[i]);
                fputs(content, file);
            }
            i++;
        }
    }
    fclose(stream);
    fclose(file);
    return 200;
//printf("[6]");
}