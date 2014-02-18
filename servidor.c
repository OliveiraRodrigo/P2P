#include "comandos.h"
#include "md5.h"
#include <pthread.h>

#define MAX_THREADS 3 // Quantas conexoes simultaneas
#define CHAVE "DiJqWHqKtiDgZySAv7ZX"

intptr_t servidor(intptr_t porta_servidor){
    
    intptr_t porta;
    struct sockaddr_in endereco_meu;
    
    /* cria socket. PF_INET define IPv4, SOCK_STREAM define TCP */
    porta = socket(PF_INET, SOCK_STREAM, 0);
    
    /* verifica se foi criado */
    if (porta == -1){
        perror("\n ::::: Erro: porta nao foi criada corretamente");
        //exit(1);
    }
    
    /* porta criada, agora faz o bind com o numero da porta desejado */
    endereco_meu.sin_family = AF_INET;
    endereco_meu.sin_port = htons(porta_servidor);
    endereco_meu.sin_addr.s_addr = INADDR_ANY;
    memset(&(endereco_meu.sin_zero), '\0', 8);
    
    if (bind(porta,(struct sockaddr *) &endereco_meu, sizeof(struct sockaddr_in)) == -1){
        perror("\n ::::: Erro: servidor nao conseguiu fazer bind.\n");
    }
    
    /* agora faz uma chamada ao listen*/
    if (listen(porta,50)==-1){
        perror("\n ::::: Erro: servidor tem problemas com o listen\n");
        //exit(1);
    }
    
    return porta;

}

/* Aguarda conexoes ***********************************************************/
void * start_connection(void* server_port){
    
    int tamanho, numbytes, fileCounter, i;
    bool repete;
    intptr_t porta_serv, nova_porta, porta_servInter;
    static int num_threads = 0;
    char ip_meu[20];
    char ip_cliente[20];
    char buffer[10000];
    static IPs ips;
    struct sockaddr_in endereco_cliente;
    pthread_t new_thread;
    protocolo protoin;
    archive_def files[100];
    bool clienteAutenticado = false;
    
    porta_serv = (intptr_t) server_port;
    tamanho = sizeof(struct sockaddr_in);
    strcpy(ip_meu, get_my_ip("eth0"));
    
    /*Teste
    ips_list(INSERT, SERVER, "111.222.333.444", NULL);
    ips_list(INSERT, SERVER, "2.3.8.444", NULL);
    ips_list(INSERT, SERVER, "199.5.55.5", NULL);*/
    
    repete = true;
    while(repete){
        
        /*Fica esperando aqui*/
        nova_porta = accept(porta_serv, (struct sockaddr*)&endereco_cliente, &tamanho);
        
        /* Ao aceitar uma conexao, cria uma nova thread para esperar nova conexao. */
        /* Se o numero de threads chegou ao limite, nao abre uma nova, mas repete 
         * a atual, quando terminar de tratar a conexao corrente. */
        if(num_threads < MAX_THREADS){
            pthread_create(&new_thread, NULL, start_connection, (void*) porta_serv);
            num_threads++;
            repete = false;
        }
        
        if(nova_porta==-1){
            perror("\n ::::: Erro: servidor: accept retornou erro\n");
            exit(1);
        }
        
        /* Pega o IP do cliente atual */
        strcpy(ip_cliente, inet_ntoa(endereco_cliente.sin_addr));
        
/* Recebe dados *******************************************************/
        if((numbytes = recv(nova_porta, buffer, 9999, 0)) == -1) {
            perror("\n ::::: Erro: Servidor nao conseguiu receber.\n");
            break;
        }
        buffer[numbytes] = '\0';
        //printf("\n ::::: Servidor recebeu: %s\n", buffer);
        protoin = set_proto(buffer);
/**********************************************************************/
        
        if(protoin.ok){
            if(!strcmp(protoin.command, "ping")){
                //printf("\n ::::: Servidor enviando pong...\n");
                if(send(nova_porta, pong(ip_meu, ip_cliente), 999, 0) == -1){
                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'pong'.");
                }
            }
            else{
                if(!strcmp(protoin.command, "authenticate")){
                    /* Envia um certify pro servidor intermediario
                     * e espera dele um certify-back */
                    
                    porta_servInter = porta(IP_SERVINTER, PORTA_SERVINTER);
                    
                    /* Babaquice ********************************************/
                    send(porta_servInter, ping(ip_meu, IP_SERVINTER), 999, 0);
                    recv(porta_servInter, buffer, 999, 0); //pong
                    close(porta_servInter);
                    porta_servInter = porta(IP_SERVINTER, PORTA_SERVINTER);
                    /********************************************************/
                    
                    if(send(porta_servInter, certify(protoin.certif.nick, protoin.certif.key, ip_meu, IP_SERVINTER), 9999, 0) == -1) {
                        perror("\n ::::: Erro: servidor nao conseguiu enviar 'certify'.");
                        clienteAutenticado = false;
                    }
                    else{
                        if((numbytes = recv(porta_servInter, buffer, 999, 0)) == -1){//certify-back
                            perror("\n ::::: Erro: Servidor nao conseguiu receber.\n");
                            clienteAutenticado = false;
                        }
                        buffer[numbytes] = '\0';
                        //printf("\n ::::: Servidor recebeu: %s\n", buffer);
                        protoin = set_proto(buffer);
                        if(protoin.ok && (protoin.status == 200)){
                            clienteAutenticado = true;
                        }
                        else{
                            clienteAutenticado = false;
                        }
                    }
                    
                    if(clienteAutenticado){
                        //printf("\n ::::: Servidor enviando authenticate-back...\n");
                        if(send(nova_porta, authenticate_back(200, ip_meu, ip_cliente), 999, 0) == -1){
                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'authenticate-back'.");
                        }
                        ips_list(INSERT, SERVER, ip_cliente, NULL);
                    }
                    else{
                        send(nova_porta, authenticate_back(203, ip_meu, ip_cliente), 999, 0);
                    }
                }
                else{
                    /* Faz de conta que sou o servidor intermediario */
                    if(!strcmp(protoin.command, "certify")){
                        if(send(nova_porta, certify_back(200, protoin.certif.nick, "certAddr", IP_SERVINTER, ip_cliente), 999, 0) == -1){
                            perror("\n ::::: Erro: servidor nao conseguiu enviar 'certify-back'.");
                        }
                        //ips_list(INSERT, SERVER, ip_cliente, NULL);
                    }
                    else{
                        /* A partir daqui so aceita se o cliente estiver logado */
                        if(ips_list(FIND, SERVER, ip_cliente, NULL)){
                            if(!strcmp(protoin.command, "agent-list")){
                                ips_list(GET, SERVER, NULL, ips);
                                if(send(nova_porta, agent_list_back(200, get_ips_string(ips), ip_meu, ip_cliente), 999, 0) == -1){
                                    perror("\n ::::: Erro: servidor nao conseguiu enviar 'agent-list-back'.");
                                }
                            }
                            else{
                                if(!strcmp(protoin.command, "archive-list")){
                                    fileCounter = setFileList("shared", files);
                                    if(send(nova_porta, archive_list_back(200, files, fileCounter, ip_meu, ip_cliente), 9999, 0) == -1){
                                        perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-list-back'.");
                                    }
                                }
                                else{
                                    if(!strcmp(protoin.command, "archive-request")){
                                        fileCounter = setFileList("shared", files);
                                        if(tem_arch(files, fileCounter, protoin.file.id)){
                                            if(send(nova_porta, archive_request_back(302, files[protoin.file.id], ip_meu, ip_cliente), 999, 0) == -1){
                                                perror("\n ::::: Erro: servidor nao conseguiu enviar 'archive-request-back'.");
                                            }
                                        }
                                        else{
                                            send(nova_porta, archive_request_back(404, files[0], ip_meu, ip_cliente), 999, 0);
                                        }
                                    }
                                    else{
                                        if(!strcmp(protoin.command, "end-connection")){
                                            ips_list(REMOVE, SERVER, ip_cliente, NULL);
                                        }
                                        else{
                                            //comando nao reconhecido
                                            send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 999, 0);
                                        }
                                    }
                                }
                            }
                        }
                        else{
                            send(nova_porta, authenticate_back(401, ip_meu, ip_cliente), 999, 0);
                        }
                    }
                }
            }
        }
        else{
            //proto nao ok
            send(nova_porta, authenticate_back(400, ip_meu, ip_cliente), 999, 0);
        }
        /*if (fork()==0){ // se for o filho
            close(porta_serv); // o filho nao aceita conexoes a mais
            //close(nova_porta);
            //exit(0); // tao logo termine, o filho pode sair
        }*/
        close(nova_porta); // essa parte somente o pai executa
        close(porta_servInter);
    }
    num_threads--;
}
