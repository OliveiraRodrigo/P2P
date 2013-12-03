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
#define SIZE 50

struct ips{
    char * ip;
} ips_list[SIZE];

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
    if(!strcmp(comando, "q")) //"quit"
        return 8;
    return 9;
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

char * insert_ip(char * novo_ip){
    
    static int i = 0;
    char * saida = (char*) malloc(100*sizeof(char));
    
    if(i < SIZE){
        ips_list[i].ip = (char*) malloc(20*sizeof(char));
        strcpy(ips_list[i].ip, novo_ip);
        sprintf(saida, "\n ::::: IP %s inserido com sucesso.\n", novo_ip);
        i++;
    }
    else{
        sprintf(saida, "\n ::::: Erro: A lista de IPs esta cheia.\n", novo_ip);
    }
    
    return saida;
}

