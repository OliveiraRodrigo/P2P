#include <stdio.h>
#include <stdlib.h>

int main(void){
    FILE *fp;
    unsigned int i, v;
    char ch;
    char nomeArquivo[50];
    sprintf(nomeArquivo, "../teste");
    
    if((fp = fopen(nomeArquivo,"r")) == NULL){
        printf("ERRO ao abrir o arquivo %s\n",nomeArquivo);
    }
    else{
        ch = getc(fp);
        while(ch != EOF){
            printf("%c",ch);
            ch = getc(fp);
        }
        fclose(fp);
    }        
}
