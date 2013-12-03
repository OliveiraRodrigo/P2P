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

typedef struct{
    char * type;        // "file" or "folder"
    int    id;          // 1, 2, 3...
    char * name;        // "nome do arquivo.txt"
    char * size;        // "100", "15487"... (KB)
    char * file;        // O arquivo em si (de algum jeito)(bin, hexa, base64...?)(char, int...?)
    char * md5;
} archive_def;

void * cliente(/*char** parametros*/);

void * servidor();

char * get_my_ip();

char * insert_ip(char * novo_ip);

char * ping(char * meu_ip, char * seu_ip);

char * pong(char * meu_ip, char * seu_ip);

char * agent_list(char * meu_ip, char * seu_ip);

char * agent_list_back();

char * authenticate(char * pass, char * ip_meu, char * ip_destino);

char * authenticate_back(int code, char * ip_meu, char * ip_destino);

char * archive_list(char * ip_meu, char * ip_destino);

char * archive_list_back(int code, archive_def * archs, char * ip_meu, char * ip_destino);

char * archive_request(char * arch_id, char * ip_meu, char * ip_destino);

char * archive_request_back(int code, archive_def arch, char * ip_meu, char * ip_destino);

char * end_connection(char * ip_meu, char * ip_destino);

