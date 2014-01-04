#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "comandos.h"

#define MAX_THREADS 10 // Quantas conexoes simultaneas

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
    
    char * saida       = (char*) malloc(2000 * sizeof(char));
    char * archs_list  = (char*) malloc(2000 * sizeof(char));
    int i;
    
    //strcpy(archs_list, " ");
    
    i = 1;
    if(quant_archs > 0){
        sprintf(archs_list, "{\"id\":%d,", archs[i].id);
        sprintf(archs_list, "%s\"name\":\"%s\",", archs_list, archs[i].name);
        sprintf(archs_list, "%s\"size\":\"%s\"}", archs_list, archs[i].size);
        i++;
    }
    while(i <= quant_archs){
        sprintf(archs_list, "%s,{\"id\":%d,", archs_list, archs[i].id);
        sprintf(archs_list, "%s\"name\":\"%s\",", archs_list, archs[i].name);
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

void get_command(char * comando[4]){
        
    int i, j;
    char temp;
    
    prompt
    
    i = 0;
    j = 0;
    temp = getchar();
    do{ //ignora as setas
        while((temp == 27)||(temp == 91)||
              (temp == 65)||(temp == 66)||
              (temp == 67)||(temp == 68)){
            temp = getchar();
        }
        comando[i][j] = temp;
        if(comando[i][j] == ' '){
            do temp = getchar();
            while(temp == ' ');
            comando[i][j] = '\0'; // Descarta o ' '.
            j = 0;
            i++;
        }
        else{
            if(comando[i][j] == '\n'){
                comando[i][j] = '\0'; // Descarta o '\n'.
                break;
            }
            else{
                temp = getchar();
                j++;
            }
        }
    }
    while(temp != '\n' && comando[i][j] != '\n');
    comando[i][j] = '\0';
    i++;
    
    while(i < 4){
        strcpy(comando[i], "\0");
        i++;
    }
}

int run_command(char ** comando, char * ip_return, char * ipdef_return, int * quit_return){
    
    if(!strcmp(comando[0], "cls")){
        if(LINUX)
            system("clear");
        clear_screen
        bg_cyan bold white printf("\n   P2P                                                                  ");
        reset
        defaults printf("\n");
        return 1;
    }
    if(!strcmp(comando[0], "ip")){
        strcpy(ip_return, get_my_ip());
        clear_line
        green printf(" P2P:> ");
        orange printf("%s\n", ip_return);
        return 1;
    }
    if(!strcmp(comando[0], "setip")){ //caso o get_my_ip nao funfe
        strcpy(ip_return, comando[1]);
        clear_line
        green printf(" P2P:> ");
        cyan printf("Ok: usando ");
        orange printf("%s", ip_return);
        cyan printf(" como meu IP.\n");
        return 1;
    }
    if(!strcmp(comando[0], "def")){ //caso o get_my_ip nao funfe
        strcpy(ipdef_return, comando[1]);
        clear_line
        green printf(" P2P:> ");
        cyan printf("Ok: usando ");
        orange printf("%s", ipdef_return);
        cyan printf(" como IP Padrao.\n");
        return 1;
    }
    if(!strcmp(comando[0], "help")){
        if(LINUX) system("clear");
        clear_screen
        
        bg_cyan bold white printf("\n   P2P                                                                  ");
        reset
        defaults printf("\n");
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
    
    if(strcmp(comando, "\0")){
        return comando;
    }
    else{
        if(strcmp(ip_default, "\0")){
            return ip_default;
        }
        else{
            clear_line
            green printf(" P2P:> ");
            red printf("Erro: IP Padrao nao configurado.\n");
            return "\0";
        }
    }
    return "\0";
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
        remove("linuxip.txt");
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
/*
int insert_ip(int quem, char ips_array[50][20], char * novo_ip){
    
    int i, size;
    
    if(quem == CLIENT)
        size = client_ips_size(0);
    else // SERVER
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
        if(quem == CLIENT)
            client_ips_size(1);
        else // SERVER
            server_ips_size(1);
        return 0;
    }
    return 1;
}

int remove_ip(int quem, char ips_array[50][20], char * target){
    
    int i, size;
    
    if(quem == CLIENT)
        size = client_ips_size(0);
    else // SERVER
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
            if(quem == CLIENT)
                client_ips_size(-1);
            else // SERVER
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
*/
int ips_list(int function, int who, char * target, IPs returnIPs){
    
    static IPs serverIPs;
    static IPs clientIPs;
    static int serverSize = 0;
    static int clientSize = 0;
    int i;
    
    switch(function){
        
        case GET:
            if(who == CLIENT){
                for(i = 0; i < clientSize; i++){
                    strcpy(returnIPs[i], clientIPs[i]);
                }
            }
            else{ //SERVER
                for(i = 0; i < serverSize; i++){
                    strcpy(returnIPs[i], serverIPs[i]);
                }
            }
            return 1;
            break;
            
        case INSERT:
            i = 0;
            if(who == CLIENT){
                while(i < clientSize){
                    //procura se ja nao tem
                    if(!strcmp(clientIPs[i], target)){
                        return -1;
                    }
                    i++;
                }
                if(i < MAXIP-1){
                    strcpy(clientIPs[i], target);
                    clientSize++;
                    return 1;
                }
                return -1;
            }
            else{ //SERVER
                while(i < serverSize){
                    //procura se ja nao tem
                    if(!strcmp(serverIPs[i], target)){
                        return -1;
                    }
                    i++;
                }
                if(i < MAXIP-1){
                    strcpy(serverIPs[i], target);
                    serverSize++;
                    return 1;
                }
                return -1;
            }
            break;
        
        case REMOVE:
            if(who == CLIENT){
                for(i = 0; i < clientSize; i++){
                    if(!strcmp(clientIPs[i], target)){
                        while(i+1 < clientSize){
                            strcpy(clientIPs[i], clientIPs[i+1]);
                            i++;
                        }
                        clientSize--;
                        return 1;
                    }
                }
                return -1;
            }
            else{ //SERVER
                for(i = 0; i < serverSize; i++){
                    if(!strcmp(serverIPs[i], target)){
                        while(i+1 < serverSize){
                            strcpy(serverIPs[i], serverIPs[i+1]);
                            i++;
                        }
                        serverSize--;
                        return 1;
                    }
                }
                return -1;
            }
            break;
        
        case FIND:
            if(who == CLIENT){
                for(i = 0; i < clientSize; i++){
                    if(!strcmp(clientIPs[i], target)){
                        return 1;
                    }
                }
                return 0;
            }
            else{ //SERVER
                for(i = 0; i < serverSize; i++){
                    if(!strcmp(serverIPs[i], target)){
                        return 1;
                    }
                }
                return 0;
            }
            break;
        
        case GETSIZE:
            if(who == CLIENT){
                return clientSize;
            }
            else{ //SERVER
                return serverSize;
            }
            break;
            
    }
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
char * get_ips_string(IPs ips_array){
    
    int i, size;
    char * saida;
    
    size = ips_list(GETSIZE, SERVER, NULL, NULL);
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
    
    for(i = 1; i <= quant_archs; i++){
        if(id == archs[i].id){
            return 1;
        }
    }
    return 0;
}

int getFileList(char *in, archive_def *files){
    
    char *field, *data;
    int f, i, j;
    
    field = (char*) malloc(20*sizeof(char));
    data  = (char*) malloc(110*sizeof(char));
    
    f = 0;
    i = 0;
    j = 0;
    while(in[i] != ']'){
        if(in[i] == '['){
            i++;
        }
        if(in[i] == '{'){
            i++;
        }
        if(in[i] == '"'){
            i++;
            while(in[i] != '"'){
                field[j] = in[i];
                i++;
                j++;
            }
            field[j] = '\0';
            j = 0;
            i++;
            if(in[i] == ':'){
                i++;
                if(in[i] == '"'){
                    i++;
                }
                while(in[i] != ',' && in[i] != '"'){
                    data[j] = in[i];
                    i++;
                    j++;
                }
                if(in[i] == '"'){
                    i++;
                }
                data[j] = '\0';
                j = 0;
            }
            if(!strcmp(field, "id"))
                files[f].id = atoi(data);
            else if(!strcmp(field, "name"))
                strcpy(files[f].name, data);
            else if(!strcmp(field, "size"))
                strcpy(files[f].size, data);
            else{
                //ERRO printf("\n-%s-\n", field);
            }
            if(in[i] == '}'){
                i++;
                f++;
            }
            if(in[i] == ','){
                i++;
            }
        }
    }
    return f;
}

protocolo set_proto(char * entrada){
    
    int i, j;
    protocolo proto;
    char * field = (char*) malloc(20*sizeof(char));
    char * data = (char*) malloc(2000*sizeof(char));
    char * seq = (char*) malloc(20*sizeof(char)); //Pra saber se esta na sequencia correta
    char * ordem = (char*) malloc(20*sizeof(char)); //Sequencia correta dependendo do comando
    i = 0;
    if(entrada[i] == '{'){
            i++;
            while(entrada[i] != '}'){
                j = 0;
                while(entrada[i] == ' '){
                    i++;
                }
                if(entrada[i] == '"'){
                    i++;
                    while(entrada[i] != '"'){
                        field[j] = entrada[i];
                        i++;
                        j++;
                    }
                    i++;
                    field[j] = '\0';
                    j = 0;
                    if(entrada[i] != ':'){
                        //erro
                        printf(" --00-- ");
                        return proto;
                    }
                    else{
                        i++;
                    }
                    if(entrada[i] == '['){
                        while(entrada[i] != ']'){
                            data[j] = entrada[i];
                            i++;
                            j++;
                        }
                        data[j] = entrada[i];
                        i++;
                        j++;
                        data[j] = '\0';
                        j = 0;
                    }
                    else{
                        if(entrada[i] == '"'){
                            i++;
                            while(entrada[i] != '"'){
                                data[j] = entrada[i];
                                i++;
                                j++;
                            }
                            i++;
                            data[j] = '\0';
                            j = 0;
                    }
                    else{
                        while(entrada[i] != ','){
                            data[j] = entrada[i];
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
                        i++;
                    }
                }
                else{
                    //erro
                    //printf(" --3-- ");
                    return proto;
                }
            }
    }
    else{
        //erro
        //printf(" --4-- ");
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
    }
    if(strcmp(seq, ordem){
        //erro
        //printf(" --6-- ");
        return proto;
    }*/
    
    proto.ok = 1;
    return proto;
}

void help(){
    
    printf("\n");
    bg_red bold white printf("\n   HELP                                                                 ");
    reset defaults printf("\n\n");
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

void * httpReq(void* porta_http){
    
    int fd, bytes_read, size, repete;
    static int num_threads = 0;
    intptr_t porta, porta_cliente;
    char mesg[10000], *reqline[3], data_to_send[BYTES], path[9999];
    struct sockaddr_in endereco_cliente;
    pthread_t new_thread;
    
    size = sizeof(struct sockaddr_in);
    porta = (intptr_t) porta_http;
    
    repete = 1;
    while(repete){
        
        porta_cliente = accept(porta, (struct sockaddr*)&endereco_cliente, &size);
        
        /*if(num_threads < MAX_THREADS){
            pthread_create(&new_thread, NULL, httpReq, (void*) porta);
            num_threads++;
            repete = 0;
        }*/
        
        if(ips_list(FIND, SERVER, inet_ntoa(endereco_cliente.sin_addr), NULL)){
            //printf("\nOK\n");
            
            recv(porta_cliente, mesg, 9999, 0);
            //printf("[%s]", mesg);
            reqline[0] = strtok (mesg, " \t\n");
            
            if(strncmp(reqline[0], "GET\0", 4)==0){
                reqline[1] = strtok (NULL, " \t");
                reqline[2] = strtok (NULL, " \t\n");
                if(strncmp(reqline[2], "HTTP/1.0", 8) != 0 && strncmp(reqline[2], "HTTP/1.1", 8) != 0){
                    write(porta_cliente, "HTTP/1.0 400 Bad Request\n", 25);
                }
                else{
                    if(strncmp(reqline[1], "/\0", 2)==0){
                        reqline[1] = "/index.html"; //index.html aberto por padrao, caso nao seja especificado nenhum arquivo.
                    }
                    strcpy(path, "shared");
                    strcpy(&path[strlen("shared")], reqline[1]);
                    //printf("file: %s\n", path);
                    if((fd=open(path, 0/*O_RDONLY*/))!=-1){
                        send(porta_cliente, "HTTP/1.0 200 OK\r\n\r\n", 19, 0);
                        while((bytes_read=read(fd, data_to_send, BYTES)) > 0){
                            write (porta_cliente, data_to_send, bytes_read);
                        }
                    }
                    else{
                        write(porta_cliente, "HTTP/1.0 404 Not Found\n", 23);
                    }
                }
            }
            else{
                write(porta_cliente, "HTTP/1.0 400 Bad Request\n", 25);
            }
        }
        else{
            //printf("\nNOT\n");
            write(porta_cliente, "HTTP/1.0 401 Unauthorized\n", 26);
        }
        close(porta_cliente);
    }
    num_threads--;
}
