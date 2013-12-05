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

#define MAX 50

typedef struct{
    char * type;        // "file" or "folder"
    int    id;          // 1, 2, 3...
    char * name;        // "nome do arquivo.txt"
    char * size;        // "100", "15487"... (KB)
    char * http;        // Endereco gerado para disponibilizacao do arquivo
    char * md5;         // Assinatura do arquivo para validacao
} archive_def;

typedef struct proto{
    char protocol[5];
    char command[20];
    int  status;
    char passport[30];
    char back[200];
    archive_def file;
    char sender[20];
    char receptor[20];
    int  ok;
} protocolo;

typedef struct ips{
    char ip[MAX][20];
    int size;
} ips_list;

void * cliente();

void * servidor();

char * ping(char * meu_ip, char * seu_ip);

char * pong(char * meu_ip, char * seu_ip);

char * agent_list(char * meu_ip, char * seu_ip);

char * agent_list_back(int code, char * ips_string, char * ip_meu, char * ip_destino);

char * authenticate(char * pass, char * ip_meu, char * ip_destino);

char * authenticate_back(int code, char * ip_meu, char * ip_destino);

char * archive_list(char * ip_meu, char * ip_destino);

char * archive_list_back(int code, archive_def * archs, char * ip_meu, char * ip_destino);

char * archive_request(char * arch_id, char * ip_meu, char * ip_destino);

char * archive_request_back(int code, archive_def arch, char * ip_meu, char * ip_destino);

char * end_connection(char * ip_meu, char * ip_destino);

int qual_comando(char * comando);

char ** get_command();

int run_command(char ** comando, char * ip_return, int * esc_sessao, int * quit);

char * get_my_ip();

int insert_ip(char * ips_string, char * novo_ip);

int remove_ip(char * ips_string, char * target);

//char ** get_ips_list();

protocolo set_proto(char * entrada);

//int set_ips_list(char * proto_back);

void help();

