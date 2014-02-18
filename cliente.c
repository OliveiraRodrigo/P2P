#include "comandos.h"
#include "md5.h"
#include <sys/stat.h>
//#include <time.h>

#define CHAVE "DiJqWHqKtiDgZySAv7ZX"
#define NICK "rodrigodroliveira"

void * cliente(){
    
    int i, quant, caract, numbytes, code;
    bool quit;
    intptr_t porta_destino;
    char buffer[10000];
    char * comando[4];
    char myPublicKey[1000];
    char ip_meu[20];
    char ip_destino[20];
    char ip_default[20];
    char truncName[80], path[200];
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
    
    /* Pega a string da minha chave publica */
    //strcpy(myPublicKey, getKey());
    
/* Recebe comandos enquanto um deles nao for "quit" ***************************/
    quit = false;
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
                            //printf("\n P2P:> %s", authenticate(NICK, /*myPublicKey*/"x", ip_meu, ip_destino));
                            
                            if(send(porta_destino, authenticate(NICK, /*myPublicKey*/"x", ip_meu, ip_destino), 999,0) != -1){
                                
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
                                                            /*green printf(" P2P:> ");
                                                            orange printf("%s", ip_destino);
                                                            cyan printf(" enviou os dados do arquivo ");
                                                            orange printf("%d", protoin.file.id);
                                                            cyan printf(":\n\n\t Link: ");
                                                            orange printf("%s", protoin.file.http);
                                                            cyan printf("\n\t MD5:  ");
                                                            orange printf("%s\n", protoin.file.md5);*/
                                                            mode_t process_mask = umask(0);
                                                            mkdir("downloads", S_IRWXU | S_IRWXG | S_IRWXO);
                                                            umask(process_mask);
                                                            code = down(ip_destino, protoin.file.http);
                                                            if(code == 200){
                                                                sprintf(path, "downloads/%s", protoin.file.http);
                                                                if(!strcmp(MD5(path), protoin.file.md5)){
                                                                    green printf(" P2P:> ");
                                                                    cyan printf("Arquivo ");
                                                                    orange printf("%s", protoin.file.http);
                                                                    cyan printf(" recebido com sucesso.\n");
                                                                }
                                                                else{
                                                                    green printf(" P2P:> ");
                                                                    red printf("Erro: arquivo ");
                                                                    orange printf("%s", protoin.file.http);
                                                                    red printf(" corrompido (MD5 nao confere).\n");
                                                                }
                                                            }
                                                            else{
                                                                green printf(" P2P:> ");
                                                                red printf("Erro: codigo %d\n", code);
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
