#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

int main()
{
    baixaArquivo("172.16.2.1", 8000, "testedownload.c");
}
int baixaArquivo(char shost[64], int porta, char url[128])
{
    char req[512]; //requisição
    int sock; //socket
    struct sockaddr_in addr;
    struct hostent  * host = NULL;
    
    char buffer[512]; // Buffer para armazenar dados

    int bytes = 1; // Bytes recebidos
    int flagAcabouCabecalho=0;
    int cont=0;
    char *pSplit=NULL;
    FILE *fp; //arquivo para escrever dados baixados
    char nomeArquivo[128];
    
    sprintf(nomeArquivo, "%s.part", url);
    fp=fopen(nomeArquivo,"w+b"); //abra arquivo
    if(fp==NULL){
       printf("erro ao criar arquivo");
       return 1;
    }

    
    // Cria socket para conectar ao servidor http
    if((sock = socket(AF_INET,SOCK_STREAM,0))<0){
        printf("ERRO ao criar socket[1]");
        return 1;
    }
    addr.sin_family=AF_INET;
    addr.sin_port = htons(porta);//seta porta
    host = gethostbyname(shost);//sera host

    if(host == NULL){ //se host não responde, cai fora
        printf("ERRO ao criar socket [2]");
        return 1;
    }
    
    //ajusta ip do host
    memcpy(&addr.sin_addr,host->h_addr_list[0],host->h_length);

    //conecta
    if(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0){
        printf("ERRO ao conectar ao servidor para download");
        return 1;
    }

    // Requisição
    sprintf(req,"GET /%s HTTP/1.1\r\n\r\n",url);//monta requisição com a url
    
    send(sock,req,strlen(req),0); // Envia requisição


    while(bytes > 0) // Enquanto estiver recebendo
    {        
        memset(buffer,0,512); // Limpa o buffer
        bytes = recv(sock,buffer,512,0); // Recebe dados
       
        if(bytes>0){
            cont++;
            printf("\nRecebendo parte %d do arquivo...\n",cont);
        }
        
        if(flagAcabouCabecalho==0){ //se o cabeçalho ainda não acabou, busca pelo fim dele
            pSplit = strstr(buffer, "\r\n\r\n"); //se achar o fim do cabeçalho, retorna ponteiro apontando para ele
            pSplit+=4;
            //printf("\n1\n");
        }
        else{
            pSplit=NULL;
            //printf("\n2\n");
        }
        if(pSplit != NULL) {
            flagAcabouCabecalho=1;//se cabeçalho acabou, seta a flag
            printf("%s",pSplit); //imprime parte do buffer que está após o cabeçalho
            fprintf(fp,"%s",pSplit); //salva parte do buffer que está após o cabeçalho
            //printf("\n3\n");
        }
        else{
            if(flagAcabouCabecalho==1){ //se cabeçalho já acabou, imprime buffer inteiro inteiro
                printf("%s",buffer);
                fprintf(fp,"%s",buffer);
                //printf("\n4\n");
            }
        }
    }
    
    sprintf(req, "mv %s %s", nomeArquivo, url);//monta string pro system a seguir
    system(req); //renomeia o arquivo para o nome original (sem ".part" que havia sido adicionado)
    
    printf("\nDownload concluído!\n");
    
    close(sock); // Fecha socket
    fclose(fp);
    
    return 0;
}
