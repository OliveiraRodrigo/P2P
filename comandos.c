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
#define MAX 50

ips_list ips;

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

char * agent_list(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"agent-list\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}

char * agent_list_back(int code, char * ips_string, char * ip_meu, char * ip_destino){
    
    char * saida     = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"agent-list-back\", "
                   "status:\"%d\", "
                   "back:\"[%s\"], "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", code, ips_string, ip_meu, ip_destino);
    
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

char * end_connection(char * ip_meu, char * ip_destino){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    //saida = " ";
    sprintf(saida, "{protocol:\"pcmj\", "
                   "command:\"end-conection\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
}

char ** get_command(){
/* Recebe os parametros caracter a caracter ***********************************/
        
    int i, j;
    char ** saida;
    
    /* Prompt */
    printf("\n P2P:> ");
    
    /* Aloca espaco para 4 parametros com 50 caracteres cada */
        saida = (char**) malloc(4*sizeof(char*));
        for(i = 0; i < 4; i++){
            saida[i] = (char*) malloc(50*sizeof(char));
        }
        
        i = 0;
        j = 0;
        saida[i][j] = getchar();
        j++;
        while(saida[i][j-1] != '\n'){
            saida[i][j] = getchar();
            if(saida[i][j] == ' '){
                saida[i][j] = '\0'; // Descarta o ' '.
                j = 0;
                i++;
            }
            else{
                j++;
            }
        }
        saida[i][j-1] = '\0'; // Descarta o '\n'.
        return saida;
}
/*
int qual_comando(char * comando){
    
    if(!strcmp(comando, "try"))
        return 0;
    if(!strcmp(comando, "login"))
        return 1;
    if(!strcmp(comando, "list"))
        return 2;
    if(!strcmp(comando, "down"))
        return 3;
    if(!strcmp(comando, "ip"))
        return 4;
    if(!strcmp(comando, "setip"))
        return 5;
    if(!strcmp(comando, "help"))
        return 97;
    if(!strcmp(comando, "q")) //"quit"
        return 98;
    return 99;
}
*/
int run_command(char ** comando, char * ip_return, int * quit_return){
    
    if(!strcmp(comando[0], "ip")){
        strcpy(ip_return, get_my_ip());
        printf("\n P2P:> %s\n", ip_return);
        return 1;
    }
    if(!strcmp(comando[0], "setip")){ //get_mey_ip nao ta funfando no Linux
        strcpy(ip_return, comando[1]);
        printf(" P2P:> Ok: usando %s como meu IP.\n", ip_return);
        return 1;
    }
    if(!strcmp(comando[0], "help")){
        help();
        return 1;
    }
    if(!strcmp(comando[0], "q")){ //"quit"
        *quit_return = 1;
        return 1;
    }
    return 0;
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
    strcpy(ip, inet_ntoa(addr.sin_addr));
    //printf("\n P2P:> %s\n", ip);
    return ip;
}

int insert_ip(char * ips_string, char * novo_ip){
    
//    static int i = 0;
//    static ips_list ips;
//    char * saida = (char*) malloc(1000*sizeof(char));
    
    if(strlen(ips_string) <= 2){
        strcpy(ips_string, novo_ip);
    }
    else{
        sprintf(ips_string, "%s,%s", ips_string, novo_ip);
    }
    
//    if(i < MAX){
//        strcpy(ips.ip[i], novo_ip);
//        i++;
//        ips.size = i;
//        sprintf(saida, "\n ::::: IP %s inserido com sucesso.\n", novo_ip);
//    }
//    else{
//        sprintf(saida, "\n ::::: Erro: A lista de IPs esta cheia.\n", novo_ip);
//    }
    
    return 0;
}

int remove_ip(char * ips_string, char * target){
    
///////////////////////////////////////////////////////
// Igual set_ips_list, com as modificacoes necessarias.
    int i, j;
    char * ip;
    char * temp;
    
    ip = (char*) malloc(20*sizeof(char));
    temp = (char*) malloc(1000*sizeof(char));
    
    strcpy(temp, ips_string); //Salva uma copia pra comparar
    strcpy(ips_string, "x"); //Zera pra reinserir tudo exceto o target
    //Eu sei que eh um baita enjambre, mas to sem tempo.
    
    i = 1;
    j =0;
    while(temp[i] != ']'){
        while((temp[i] != ',') && (temp[i] != ']')){
            ip[j] = temp[i];
            i++;
            j++;
            if(j >= 20){
                break;
            }
        }
        ip[j] = '\0';
        if(strcmp(ip, target)){
            insert_ip(temp, ip);
        }
        i++;
        j = 0;
        if(i >= strlen(temp)){
            break;
        }
    }
///////////////////////////////////////////////////////
    return 0;
}
/*
char ** get_ips_list(){
    
    int i;
    char ** saida;

//    char * ips_list  = (char*) malloc(200 * sizeof(char));
//    int i;
//    i = 0;
//    sprintf(ips_list, "%s", ips[i]);
//    i++;
//    ips++;
//    while(**ips){
//        sprintf(ips_list, "%s,%s", ips_list, ips[i]);
//        i++;
//        ips++;
//    }
    
    saida = (char**) malloc(ips.size*sizeof(char*));
    for(i = 0; i < ips.size; i++){
        printf("\n:: %d", i);
        saida[i] = (char*) malloc(20*sizeof(char));
        strcpy(saida[i], ips.ip[i]);
    }
    
    return saida;
}
*/
protocolo set_proto(char * entrada){
    
    int i, j;
    protocolo proto;
    char * field = (char*) malloc(20*sizeof(char));
    char * data = (char*) malloc(200*sizeof(char));
    char * seq = (char*) malloc(20*sizeof(char)); //Pra saber se esta na sequencia correta
    char * ordem = (char*) malloc(20*sizeof(char)); //Sequencia correta dependendo do comando
    
    i = 0;
    if(entrada[i] == '{'){
//printf("%c",entrada[i]);
            i++;
            while(entrada[i] != '}'){
                j = 0;
                while(entrada[i] == ' '){
//printf("%c",entrada[i]);
                    i++;
                }
                while(entrada[i] != ':'){
//printf("%c",entrada[i]);
                    field[j] = entrada[i];
                    i++;
                    j++;
                }
//printf("%c",entrada[i]);
                i++;
                field[j] = '\0';
                j = 0;
                if(entrada[i] == '"'){
//printf("%c",entrada[i]);
                    i++;
                    while(entrada[i] != '"'){
                        if(entrada[i] == '['){
                            while(entrada[i] != ']'){
                                data[j] = entrada[i];
//printf("%c",entrada[i]);
                                i++;
                                j++;
                            }
                        }
                        data[j] = entrada[i];
//printf("%c",entrada[i]);
                        i++;
                        j++;
                    }
//printf("%c",entrada[i]);
                    i++;
                    data[j] = '\0';
                    j = 0;
                    if(!strcmp(field, "protocol")){
                        strcpy(proto.protocol, data);
                        strcat(seq, "p");
                    }
                    else{
                        if(!strcmp(field, "command")){
                            strcpy(proto.command, data);
                            strcat(seq, "c");
                        }
                        else{
                            if(!strcmp(field, "status")){
                                proto.status = atoi(data);
                                strcat(seq, "t");
                            }
                            else{
                                if(!strcmp(field, "passport")){
                                    strcpy(proto.passport, data);
                                    strcat(seq, "k");
                                }
                                else{
                                    if(!strcmp(field, "back")){
                                        strcpy(proto.back, data);
                                        strcat(seq, "b");
                                    }
                                    else{
                                        if(!strcmp(field, "id")){
                                            proto.file.id = atoi(data);
                                            strcat(seq, "i");
                                        }
                                        else{
                                            if(!strcmp(field, "http_address")){
                                                strcpy(proto.file.http, data);
                                                strcat(seq, "h");
                                            }
                                            else{
                                                if(!strcmp(field, "size")){
                                                    strcpy(proto.file.size, data);
                                                    strcat(seq, "z");
                                                }
                                                else{
                                                    if(!strcmp(field, "md5")){
                                                        strcpy(proto.file.md5, data);
                                                        strcat(seq, "m");
                                                    }
                                                    else{
                                                        if(!strcmp(field, "sender")){
                                                            strcpy(proto.sender, data);
                                                            strcat(seq, "s");
                                                        }
                                                        else{
                                                            if(!strcmp(field, "receptor")){
                                                                strcpy(proto.receptor, data);
                                                                strcat(seq, "r");
                                                            }
                                                            else{
                                                                //erro
                                                                //printf(" --1-- ");
                                                                return proto;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if(entrada[i] == ','){
//printf("%c",entrada[i]);
                        i++;
                    }
                }
                else{
                    //erro
                    //printf(" --3-- ");
                    return proto;
                }
            }
//printf("%c",entrada[i]);
    }
    else{
        //erro
        //printf(" --4-- ");
        return proto;
    }
    
    //Testa a ordenacao do protocolo
    if(!strcmp(proto.command,"ping")){
        strcpy(ordem, "pcsr");
    }
    else{
        if(!strcmp(proto.command,"pong")){
            strcpy(ordem, "pctsr");
        }
        else{
            if(!strcmp(proto.command,"authenticate")){
                strcpy(ordem, "pcksr");
            }
            else{
                if(!strcmp(proto.command,"authenticate-back")){
                    strcpy(ordem, "pctsr");
                }
                else{
                    if(!strcmp(proto.command,"agent-list")){
                        strcpy(ordem, "pcsr");
                    }
                    else{
                        if(!strcmp(proto.command,"agent-list-back")){
                            strcpy(ordem, "pctbsr");
                        }
                        else{
                            if(!strcmp(proto.command,"archive-list")){
                                strcpy(ordem, "pcsr");
                            }
                            else{
                                if(!strcmp(proto.command,"archive-list-back")){
                                    strcpy(ordem, "pctbsr");
                                }
                                else{
                                    if(!strcmp(proto.command,"archive-request")){
                                        strcpy(ordem, "pcisr");
                                    }
                                    else{
                                        if(!strcmp(proto.command,"archive-request-back")){
                                            strcpy(ordem, "pctihzmsr");
                                        }
                                        else{
                                            if(!strcmp(proto.command,"end-conection")){
                                                strcpy(ordem, "pcsr");
                                            }
                                            else{
                                                //erro
                                                //printf(" --5-- ");
                                                return proto;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    proto.ok = 1;
    return proto;
}
/*
int set_ips_list(char * proto_back){
    
    int i, j;
    char * ip;
    
    ip = (char*) malloc(20*sizeof(char));
    
    i = 1;
    j =0;
    while(proto_back[i] != ']'){
        while((proto_back[i] != ',') && (proto_back[i] != ']')){
            ip[j] = proto_back[i];
            i++;
            j++;
            if(j >= 20){
                break;
            }
        }
        ip[j] = '\0';
        insert_ip(ip);
        i++;
        j = 0;
        if(i >= strlen(proto_back)){
            break;
        }
    }
    return 1;
}
*/
void help(){
    
    printf("\n\n");
    printf("\n== HELP ====================================\n");
    printf("\n"
           " try <IP>             Manda um ping pra <IP> e espera dele um pong.\n"
           "\n"
           " login                Tenta autenticar-se com o mesmo IP.\n"
           "\n"
           " list-users           Solicita a lista de usuarios do peer atual.\n"
           "                      Insere na lista local os IPs recebidos e retorna\n"
           "                      a lista para o usuario. Precisa estar logado.\n"
           "\n"
           " list-files           Solicita a lista de arquivos do peer atual.\n"
           "                      Retorna a lista para o usuario. Precisa estar logado.\n"
           "\n"
           " down <id>            Solicita ao peer atual o arquivo de id:\"<id>\".\n"
           "                      Se confirmada a disponibilidade, recebe o arquivo.\n"
           "                      Precisa estar logado.\n"
           "\n"
           " logout               Desconecta-se do peer atual e solicita a exclusao\n"
           "                      da lista de IPs deste.\n"
           "\n"
           " quit                 Executa logout e sai do programa.\n"
           "\n");
    printf("\n __________________________________________\n");
    
}