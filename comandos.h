#ifndef COMANDOS_H
#define	COMANDOS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* COMANDOS_H */

#if defined(__linux__)
#define LINUX 1
#else
#define LINUX 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <resolv.h>

#define bool  char
#define false 0
#define true  1

/* Fonte */
#define black   printf("\033[30m");
#define red     printf("\033[31m");
#define green   printf("\033[32m");
#define orange  printf("\033[33m");
#define blue    printf("\033[34m");
#define pink    printf("\033[35m");
#define cyan    printf("\033[36m");
#define white   printf("\033[37m");
#define bold    printf("\033[1m");
#define under   printf("\033[4m");
#define reset   printf("\033[0m");

/* Fundo */
#define bg_black  printf("\033[40m");
#define bg_red    printf("\033[41m");
#define bg_green  printf("\033[42m");
#define bg_orange printf("\033[43m");
#define bg_blue   printf("\033[44m");
#define bg_pink   printf("\033[45m");
#define bg_cyan   printf("\033[46m");
#define bg_white  printf("\033[47m");

#define defaults printf("\033[39;49m");
#define clear_line printf("\033[K");

#define clear_screen if(LINUX) system("clear");\
                     printf("\033[2J");

#define prompt bold green printf("\n P2P:> "); reset defaults\
               cyan printf("\n________________________________________________________________________\n");\
               printf("\033[2A\033[7C"); white

#define CLIENT  0
#define SERVER  1
#define GET     0
#define INSERT  1
#define REMOVE  2
#define FIND    3
#define GETSIZE 4

#define MAXIP 50 // Quantos IPs na lista
#define BYTES 1024
#define CONNMAX 10

#define PORTA_SERVIDOR  6789
#define PORTA_SERVINTER 6789
#define PORTA_HTTP      5789
#define IP_SERVINTER    "127.0.0.1"

typedef struct{
    int    id;          // 1, 2, 3...
    char name[100];     // "nome do arquivo.txt"
    char size[20];      // "100.00", "15487.32"... (KB)
    char http[100];     // Endereco gerado para disponibilizacao do arquivo
    char md5[50];       // Assinatura do arquivo para validacao
} archive_def;

typedef struct{
    char nick[100];  // Nickname
    char key[1000];  // Chave publica
    char addr[100];  // Endereco para baixar o certificado
} certif_def;

typedef struct proto{
    char protocol[5];     // Nome do protocolo
    char command[30];     // Identificacao do comando
    certif_def certif;    // Informacoes sobre arquivo
    int  status;          // Codigo de erro ou validacao
    char back[2000];      // Dado retornado
    archive_def file;     // Informacoes sobre arquivo
    char sender[20];      // IP Remetente
    char recipient[20];   // IP Destinatario
    int  ok;              // Construcao correta do protocolo
} protocolo;

typedef char IPs[MAXIP][20];

void * cliente();

intptr_t servidor(intptr_t porta_servidor);

void * start_connection(void * porta);

char * ping(char * meu_ip, char * seu_ip);

char * pong(char * meu_ip, char * seu_ip);

char * authenticate(char * myNick, char * myPublicKey, char * ip_sender, char * ip_recipient);

char * authenticate_back(int code, char * ip_meu, char * ip_destino);

char * certify(char * nick, char * publicKey, char * ip_sender, char * ip_recipient);

char * certify_back(int code, char * nick, char * certificate, char * ip_sender, char * ip_recipient);

char * agent_list(char * meu_ip, char * seu_ip);

char * agent_list_back(int code, char * ips_string, char * ip_meu, char * ip_destino);

char * archive_list(char * ip_meu, char * ip_destino);

char * archive_list_back(int code, archive_def * archs, int quant_archs, char * ip_sender, char * ip_recipient);

char * archive_request(char * arch_id, char * ip_meu, char * ip_destino);

char * archive_request_back(int code, archive_def arch, char * ip_meu, char * ip_destino);

char * end_connection(char * ip_meu, char * ip_destino);

intptr_t porta(char * ip_destino, intptr_t porta_remota);

int ssl_connect(intptr_t tcp_port, char * dest_url);

void get_command(char * comando[4]);

bool run_command(char ** comando, char * ip_return, char * ipdef_return, char * myNick, bool * quit);

char * set_ipdestino(char * comando, char * ip_default);

char * get_my_ip(char * interface);

char * get_ips_string(char ips_string[MAXIP][20]);

//bool set_ips_array(char ips_string[MAXIP][20], char * proto_back);

int ips_list(int function, int who, char * target, IPs returnIPs);

bool tem_arch(archive_def * archs, int quant_archs, int id);

int getFileList(char * protoin, archive_def * files);

int setFileList(char folder[100], archive_def * files);

protocolo set_proto(char * entrada);

int down(char ip[20], char url[128]);

void * httpReq(void * porta_http);

char * getKey(char * nickName);

void help();
