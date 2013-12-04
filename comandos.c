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
                   "command:\"end-connection\", "
                   "sender:\"%s\", "
                   "receptor:\"%s\"}", ip_meu, ip_destino);
    
    return saida;
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
    if(!strcmp(comando, "ip"))
        return 4;
    if(!strcmp(comando, "setip"))
        return 5;
    if(!strcmp(comando, "q")) //"quit"
        return 98;
    return 99;
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
    ip = inet_ntoa(addr.sin_addr);
    //printf("\n meu ip: %s ", ip_meu);
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
    
    //Zera tudo
    strcpy(proto.protocol, "x");
    strcpy(proto.command, "x");
    proto.status = 0;
    strcpy(proto.passport, "x");
    strcpy(proto.back, "x");
    strcpy(proto.sender, "x");
    strcpy(proto.receptor, "x");
    proto.ok = 0;
    
    i = 0;
    if(entrada[i] == '{'){
printf("%c",entrada[i]);
            i++;
printf("%c",entrada[i]);
            while(entrada[i] != '}'){
                j = 0;
                while(entrada[i] == ' '){
                    i++;
printf("%c",entrada[i]);
                }
                while(entrada[i] != ':'){
                    field[j] = entrada[i];
                    i++;
                    j++;
printf("%c",entrada[i]);
                }
                i++;
printf("%c",entrada[i]);
                field[j] = '\0';
                j = 0;
                if(entrada[i] == '"'){
                    i++;
printf("%c",entrada[i]);
                    while(entrada[i] != '"'){
                        data[j] = entrada[i];
                        i++;
                        j++;
printf("%c",entrada[i]);
                    }
                    i++;
printf("%c",entrada[i]);
                    data[j] = '\0';
                    j = 0;
                    if(!strcmp(field, "protocol")){
                        //proto.protocol = (char*) malloc(10);
                        strcpy(proto.protocol, data);
printf("%s",data);
                    }
                    else{
                        if(!strcmp(field, "command")){
                            //proto.protocol = (char*) malloc(20);
                            strcpy(proto.command, data);
printf("%s",data);
                        }
                        else{
                            if(!strcmp(field, "status")){
                                proto.status = atoi(data);
printf("%d",atoi(data));
                            }
                            else{
                                if(!strcmp(field, "passport")){
                                    //proto.protocol = (char*) malloc(32);
                                    strcpy(proto.passport, data);
printf("%s",data);
                                }
                                else{
                                    if(!strcmp(field, "back")){
                                        //proto.protocol = (char*) malloc(200);
                                        strcpy(proto.back, data);
                                        //archive_request_back eh um problema
printf("%s",data);
                                    }
                                    else{
                                        if(!strcmp(field, "sender")){
                                            //proto.protocol = (char*) malloc(20);
                                            strcpy(proto.sender, data);
printf("%s",data);
                                        }
                                        else{
                                            if(!strcmp(field, "receptor")){
                                              //proto.protocol = (char*) malloc(20);
                                                strcpy(proto.receptor, data);
printf("%s",data);
                                            }
                                            else{
                                                //erro
                                                return proto;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    while(entrada[i] != ','){
                        if(entrada[i] == ' '){
                            i++;
printf("%c",entrada[i]);
                        }
                        else{
                            //erro
                            return proto;
                        }
                    }
                    i++;
printf("%c",entrada[i]);
                }
            }
    }
    else{
        //erro
        return proto;
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