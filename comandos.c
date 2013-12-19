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

char * ping(char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"ping\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", ip_sender, ip_recipient);
    
    //printf("%s", saida);
    return saida;
}

char * pong(char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\", "
                   "\"command\":\"pong\", "
                   "\"sender\":\"%s\", "
                   "\"receptor\":\"%s\", "
                   "\"status\":\"100\"}\n", ip_sender, ip_recipient);
    
    return saida;
}

char * authenticate(char * pass, char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"authenticate\","
                   "\"passport\":\"%s\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", pass, ip_sender, ip_recipient);
    
    //printf("%s", saida);
    return saida;
}

char * authenticate_back(int code, char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"authenticate-back\","
                   "\"status\":\"%d\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", code, ip_sender, ip_recipient);
    
    //printf("%s", saida);
    return saida;
}

char * agent_list(char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"agent-list\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", ip_sender, ip_recipient);
    
    return saida;
}

char * agent_list_back(int code, char * ips_string, char * ip_sender, char * ip_recipient){
    
    char * saida     = (char*) malloc(1000 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"agent-list-back\","
                   "\"status\":\"%d\","
                   "\"back\":[%s],"
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", code, ips_string, ip_sender, ip_recipient);
    
    return saida;
}

char * archive_list(char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"archive-list\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", ip_sender, ip_recipient);
    
    return saida;
}

char * archive_list_back(int code, archive_def * archs, int quant_archs, char * ip_sender, char * ip_recipient){
    
    char * saida       = (char*) malloc(2000  * sizeof(char));
    char * archs_list  = (char*) malloc(2000 * sizeof(char));
    int i;
    
    strcpy(archs_list, " ");
    
    i = 0;
    if(quant_archs > 0){
        sprintf(archs_list, "{\"id\":%d,", archs[i].id);
        sprintf(archs_list, "%s\"nome\":\"%s\",", archs_list, archs[i].name);
        sprintf(archs_list, "%s\"size\":\"%s\"}", archs_list, archs[i].size);
        i++;
    }
    while(i < quant_archs){
        sprintf(archs_list, "%s,{\"id\":%d,", archs_list, archs[i].id);
        sprintf(archs_list, "%s\"nome\":\"%s\",", archs_list, archs[i].name);
        sprintf(archs_list, "%s\"size\":\"%s\"}", archs_list, archs[i].size);
        i++;
    }
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"archive-list-back\","
                   "\"status\":\"%d\","
                   "\"back\":[%s],"
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", code, archs_list, ip_sender, ip_recipient);
    
    return saida;
}

char * archive_request(char * arch_id, char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"archive-request\","
                   "\"id\":%s,"
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", arch_id, ip_sender, ip_recipient);
    
    return saida;
}

char * archive_request_back(int code, archive_def arch, char * ip_sender, char * ip_recipient){
    
    char * saida = (char*) malloc(1000  * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"archive-request-back\","
                   "\"status\":\"%d\","
                   "\"id\":%d,"
                   "\"http_address\":\"%s\","
                   "\"size\":\"%s\"," //redundante
                   "\"md5\":\"%s\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n",
                   code, arch.id, arch.http, arch.size, arch.md5, ip_sender, ip_recipient);
    
    return saida;
}

char * end_connection(char * ip_sender, char * ip_recipient){
    
    char * saida  = (char*) malloc(200 * sizeof(char));
    
    sprintf(saida, "{\"protocol\":\"pcmj\","
                   "\"command\":\"end-connection\","
                   "\"sender\":\"%s\","
                   "\"receptor\":\"%s\"}\n", ip_sender, ip_recipient);
    
    return saida;
}

char ** get_command(){
/* Recebe os parametros caracter a caracter ***********************************/
        
    int i, j;
    char temp, **saida;
    
    /* Prompt
    bold green printf("\n P2P:> "); reset white
    bg_black cyan printf("\n______________________________________________________________________\n");
    printf("\033[2A\033[7C"); white // Reposiciona o cursor*/
    prompt
    
    /* Aloca espaco para 4 parametros com 50 caracteres cada */
        saida = (char**) malloc(4*sizeof(char*));
        for(i = 0; i < 4; i++){
            saida[i] = (char*) malloc(50*sizeof(char));
        }
        
        i = 0;
        j = 0;
        temp = getchar();
        do{ //ignora as setas
            while((temp == 27)||(temp == 91)||
                  (temp == 65)||(temp == 66)||
                  (temp == 67)||(temp == 68)){
                temp = getchar();
            }
            saida[i][j] = temp;
            if(saida[i][j] == ' '){
                do temp = getchar();
                while(temp == ' ');
                saida[i][j] = '\0'; // Descarta o ' '.
                j = 0;
                i++;
            }
            else{
                if(saida[i][j] == '\n'){
                    saida[i][j] = '\0'; // Descarta o '\n'.
                    break;
                }
                else{
                    temp = getchar();
                    j++;
                }
            }
        }
        while(temp != '\n' && saida[i][j] != '\n');
        saida[i][j] = '\0';
        
        return saida;
}

int run_command(char ** comando, char * ip_return, char * ipdef_return, int * quit_return){
    
    if(!strcmp(comando[0], "cls")){
        if(LINUX) system("clear");
        clear_screen
        
        bg_cyan bold white printf("\n   P2P                                                                  ");
        reset /*bg_black*/defaults printf("\n");
        return 1;
    }
    if(!strcmp(comando[0], "ip")){
        strcpy(ip_return, get_my_ip());
        clear_line
        green printf(" P2P:> "); orange printf("%s\n", ip_return);
        return 1;
    }
    if(!strcmp(comando[0], "setip")){ //caso o get_my_ip nao funfe
        strcpy(ip_return, comando[1]);
        clear_line
        green printf(" P2P:> ");cyan printf("Ok: usando ");orange printf("%s", ip_return);cyan printf(" como meu IP.\n");
        return 1;
    }
    if(!strcmp(comando[0], "def")){ //caso o get_my_ip nao funfe
        strcpy(ipdef_return, comando[1]);
        clear_line
        green printf(" P2P:> ");cyan printf("Ok: usando ");orange printf("%s", ipdef_return);cyan printf(" como IP Padrao.\n");
        return 1;
    }
    if(!strcmp(comando[0], "help")){
        if(LINUX) system("clear");
        clear_screen
        
        bg_cyan bold white printf("\n   P2P                                                                  ");
        reset /*bg_black*/defaults printf("\n");
        help();
        return 1;
    }
    if(!strcmp(comando[0], "quit")){
	defaults        
        clear_screen
        printf("\n\n");
        reset
        *quit_return = 1;
        return 1;
    }
    if(!strcmp(comando[0], "q")){ //"quit"
        defaults
        clear_screen
        printf("\n\n");
        reset
        *quit_return = 1;
        return 1;
    }
    return 0;
}

char * set_ipdestino(char * comando, char * ip_default){
    
    char * ip_destino = (char*) malloc(20*sizeof(char));
    
    if(strlen(comando)){
        return comando;
    }
    else{
        if(strcmp(ip_default, "0")){
            return ip_default;
        }
        else{
            clear_line
            green printf(" P2P:> ");
            red printf("Erro: IP Padrao nao configurado.\n");
            return "0";
        }
    }
    return "0";
}

char * get_my_ip(){
    
    char host_meu[128];
    struct hostent *he;
    struct sockaddr_in addr;
    char * ip = (char*) malloc(20*sizeof(char));
    
    if(LINUX){
        FILE * fp;
        system("ifconfig eth0 | grep \"inet end\" | awk -F: '{print $2}' | awk '{print $1}' > linuxip.txt");
        fp = fopen("linuxip.txt", "r");
        while(fscanf(fp, "%s", ip) != EOF);
        fclose(fp);
    }
    else{
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
    }
    return ip;
}

int insert_ip(int quem, char ips_array[50][20], char * novo_ip){
    
    int i, size;
    
    if(quem == 0) //cliente
        size = client_ips_size(0);
    else //servidor
        size = server_ips_size(0);
    
    i = 0;
    while(i < size){
        //procura se ja nao tem
        if(!strcmp(ips_array[i], novo_ip)){
            return 1;
        }
        else{
            i++;
        }
    }
    
    i = size;
    if(i < MAX-1){
        strcpy(ips_array[i], novo_ip);
        if(quem == 0)
            client_ips_size(1);
        else
            server_ips_size(1);
        return 0;
    }
    return 1;
}

int remove_ip(int quem, char ips_array[50][20], char * target){
    
    int i, size;
    
    if(quem == 0) //cliente
        size = client_ips_size(0);
    else //servidor
        size = server_ips_size(0);
    
    if(size == 0){
        return 1;
    }
    
    for(i = 0; i < size; i++){
        if(!strcmp(ips_array[i], target)){
            while(i+1 < size){
                strcpy(ips_array[i], ips_array[i+1]);
                i++;
            }
            if(quem == 0)
                client_ips_size(-1);
            else
                server_ips_size(-1);
            return 0;
        }
    }
    return 1;
}

int server_find_ip(char ips_array[50][20], char * target){
    
    int i = 0;
    
    while(i < server_ips_size(0)){
        if(!strcmp(ips_array[i], target)){
            return 1;
        }
        else{
            i++;
        }
    }
    return 0;
}

int server_ips_size(int modifier){
    static int i = 0;
    i += modifier;
    return i;
}

int client_ips_size(int modifier){
    static int i = 0;
    i += modifier;
    return i;
}
/* Desnecessaria
int set_ips_array(char ips_array[50][20], char * proto_back){
    
    int i, j, size;
    char ip[20];
    
    size = strlen(proto_back);
    
    i = 1; // Ja descarta o '['
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
        insert_ip(ips_array, ip);
        i++;
        j = 0;
        if(i >= size){
            break;
        }
    }
    return 0;
}
*/
char * get_ips_string(char ips_array[50][20]){
    
    int i, size;
    char * saida;
    
    size = server_ips_size(0);
    saida = (char*) malloc(1000*size*sizeof(char));

    i = 0;
    sprintf(saida, "\"%s\"", ips_array[i]);
    i++;
    while(i < size){
        sprintf(saida, "%s,\"%s\"", saida, ips_array[i]);
        i++;
    }
    
    return saida;
}

int tem_arch(archive_def * archs, int quant_archs, int id){
    
    int i;
    
    for(i = 0; i < quant_archs; i++){
        if(id == archs[i].id){
            return 1;
        }
    }
    return 0;
}

protocolo set_proto(char * entrada){
    
    int i, j;
    protocolo proto;
    char * field = (char*) malloc(20*sizeof(char));
    char * data = (char*) malloc(200*sizeof(char));
    char * seq = (char*) malloc(20*sizeof(char)); //Pra saber se esta na sequencia correta
    char * ordem = (char*) malloc(20*sizeof(char)); //Sequencia correta dependendo do comando
//printf("\nHere we go!\n");
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
                if(entrada[i] == '"'){
//printf("%c",entrada[i]);
                    i++;
                    while(entrada[i] != '"'){
                        field[j] = entrada[i];
//printf("%c",entrada[i]);
                        i++;
                        j++;
                    }
//printf("%c",entrada[i]);
                    i++;
                    field[j] = '\0';
                    j = 0;
                    if(entrada[i] != ':'){
//printf("%c",entrada[i]);
                        //erro
                        printf(" --00-- ");
                        return proto;
                    }
                    else{
//printf("%c",entrada[i]);
                        i++;
                    }
                    if(entrada[i] == '['){
                        while(entrada[i] != ']'){
                            data[j] = entrada[i];
//printf("%c",entrada[i]);
                            i++;
                            j++;
                        }
                        data[j] = entrada[i];
//printf("%c",entrada[i]);
                        i++;
                        j++;
                        data[j] = '\0';
                        j = 0;
                    }
                    else{
                        if(entrada[i] == '"'){
//printf("%c",entrada[i]);
                            i++;
                            while(entrada[i] != '"'){
                                data[j] = entrada[i];
//printf("%c",entrada[i]);
                                i++;
                                j++;
                            }
//printf("%c",entrada[i]);
                            i++;
                            data[j] = '\0';
                            j = 0;
                    }
                    else{
                        while(entrada[i] != ','){
                            data[j] = entrada[i];
//printf("%c",entrada[i]);
                            i++;
                            j++;
                        }
                        //Aqui NAO incrementa o i, pois vai ser incrementado quando achar a virgula, mais abaixo.
                        data[j] = '\0';
                        j = 0;
                    }
                }
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
                                                                strcpy(proto.recipient, data);
                                                                strcat(seq, "r");
                                                            }
                                                            else{
                                                                //erro
//printf("%s",field);
                                                                printf(" --1-- ");
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
//printf("%c",entrada[i]);
                    //erro
                    printf(" --3-- ");
                    return proto;
                }
            }
    }
    else{
//printf("%c",entrada[i]);
        //erro
        printf(" --4-- ");
        return proto;
    }
    
    //Testa a ordenacao do protocolo
    /*Mas depois me disseram que nao importa a ordem, VTNC
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
    }*/
    
//printf("%c",entrada[i]);
//printf("\nHere we came!\n");
    proto.ok = 1;
    return proto;
}

void help(){
    
    printf("\n");
    bg_red bold white printf("\n   HELP                                                                 ");
    reset /*bg_black*/defaults printf("\n\n");
    white printf(" def "); orange printf("<IP>");
    white printf("           Define <IP> como IP Padrao, para que nao seja neces-\n");
    white printf("                    saria a sua digitacao nos proximos comandos.\n\n");
    white printf(" try "); orange printf("<IP>");
    white printf("           Testa a disponibilidade de <IP> e a retorna para o\n");
    white printf("                    usuario.\n\n");
    white printf(" login "); orange printf("<IP>");
    white printf("         Tenta autenticar-se com <IP>.\n\n");
    white printf(" list-users "); orange printf("<IP>");
    white printf("    Solicita a lista de usuarios de <IP> e a retorna pa-\n");
    white printf("                    ra o usuario.\n\n");
    white printf(" list-files "); orange printf("<IP>");
    white printf("    Solicita a lista de arquivos de <IP> e a retorna pa-\n");
    white printf("                    ra o usuario.\n\n");
    white printf(" down "); orange printf("<id> <IP>");
    white printf("     Solicita a <IP> o arquivo <id>. Se confirmada a dis-\n");
    white printf("                    ponibilidade, recebe o arquivo e o MD5 do mesmo.\n\n");
    white printf(" logout "); orange printf("<IP>");
    white printf("        Desconecta-se de <IP> e solicita a exclusao deste\n");
    white printf("                    peer da sua lista de usuarios.\n\n");
    white printf(" quit ");
    white printf("              Desconecta-se de todos os usuarios com os quais ain-\n");
    white printf("                    da esta conectado e sai do programa.\n\n");
    white printf(" cls ");
    white printf("               Limpa a tela\n");
    red printf("\n________________________________________________________________________\n");
    
}
