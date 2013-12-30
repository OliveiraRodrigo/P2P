
/* 
 * File:   comandos.h
 * Author: Rodrigo
 *
 * Created on 28 de Novembro de 2013, 23:31
 */

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

/* Fundo */
#define bg_black  printf("\033[40m");
#define bg_red    printf("\033[41m");
#define bg_green  printf("\033[42m");
#define bg_orange printf("\033[43m");
#define bg_blue   printf("\033[44m");
#define bg_pink   printf("\033[45m");
#define bg_cyan   printf("\033[46m");
#define bg_white  printf("\033[47m");

#define reset printf("\033[0m");
#define defaults printf("\033[39;49m");
#define clear_line printf("\033[K");
#define clear_screen if(LINUX){system("clear");} printf("\033[2J");
#define prompt bold green printf("\n P2P:> "); reset /*bg_black*/defaults cyan printf("\n________________________________________________________________________\n"); printf("\033[2A\033[7C"); white

#define CLIENT 0
#define SERVER 1
#define MAX 50

typedef struct{
    int    id;          // 1, 2, 3...
    char name[100];      // "nome do arquivo.txt"
    char size[10];      // "100", "15487"... (KB)
    char http[100];     // Endereco gerado para disponibilizacao do arquivo
    char md5[50];       // Assinatura do arquivo para validacao
} archive_def;

typedef struct proto{
    char protocol[5];   // Nome do protocolo
    char command[20];   // Identificacao do comando
    int  status;        // Codigo de erro ou validacao
    char passport[30];  // Chave de autenticacao
    char back[2000];     // Dado retornado
    archive_def file;   // Informacoes sobre arquivo
    char sender[20];    // IP Remetente
    char recipient[20]; // IP Destinatario
    int  ok;            // Construcao correta do protocolo
} protocolo;

void * cliente();

int porta(char * ip_destino);

int baixaArquivo(char shost[64], int porta, char url[128]);

int servidor();

void * start_connection(void * porta);

int setFileList(char folder[100], archive_def * files);

char * ping(char * meu_ip, char * seu_ip);

char * pong(char * meu_ip, char * seu_ip);

char * agent_list(char * meu_ip, char * seu_ip);

char * agent_list_back(int code, char * ips_string, char * ip_meu, char * ip_destino);

char * authenticate(char * pass, char * ip_meu, char * ip_destino);

char * authenticate_back(int code, char * ip_meu, char * ip_destino);

char * archive_list(char * ip_meu, char * ip_destino);

char * archive_list_back(int code, archive_def * archs, int quant_archs, char * ip_sender, char * ip_recipient);

char * archive_request(char * arch_id, char * ip_meu, char * ip_destino);

char * archive_request_back(int code, archive_def arch, char * ip_meu, char * ip_destino);

char * end_connection(char * ip_meu, char * ip_destino);

void get_command(char * comando[4]);

int run_command(char ** comando, char * ip_return, char * ipdef_return, int * quit);

char * set_ipdestino(char * comando, char * ip_default);

char * get_my_ip();

/* quem: 0 = lista de IPs do cliente  (macro: CLIENT),
 *       1 = lista de IPs so servidor (macro: SERVER) */
int insert_ip(int quem, char ips_string[50][20], char * novo_ip);
int remove_ip(int quem, char ips_string[50][20], char * target);

char * get_ips_string(char ips_string[50][20]);

int tem_arch(archive_def * archs, int quant_archs, int id);

int getFileList(char * protoin, archive_def * files);

protocolo set_proto(char * entrada);

//int set_ips_array(char ips_string[50][20], char * proto_back);

int server_ips_size(int modifier);

int client_ips_size(int modifier);

int server_find_ip(char ips_array[50][20], char * target);

void help();

