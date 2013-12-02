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

void * cliente(/*char** parametros*/);

void * servidor();

char * agent_list(char * meu_ip, char * seu_ip);

char * agent_list_back();

char * ping(char * meu_ip, char * seu_ip);

char * pong(char * meu_ip, char * seu_ip);