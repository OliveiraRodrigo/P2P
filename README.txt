Universidade Federal de Pelotas
Ci�ncia da Computa��o
Redes de Computadores 1
Trabalho 1: Compartilhamento P2P

Lidiane Costa, Maureen Souza, Rodrigo Oliveira

=========================================================================================================


-- Descri��o --------------------------------------------------------------------------------------------

O trabalho consiste em um compartilhador de arquivos usu�rio/usu�rio que utiliza um protocolo pr�prio,
chamado de PCMJ.

O programa conta com uma fun��o cliente, para que seja poss�vel solicitar e receber os arquivos de
outros usu�rios, e uma fun��o servidor, para enviar arquivos aos usu�rios que os solicitarem.

Al�m da fun��o servidor, que trata os comandos recebidos dos clientes, h� tamb�m uma fun��o que roda
um servidor HTTP, pois foi definido na implementa��o do protocolo que o envio/recebimento dos arquivos
deve ser feito via protocolo HTTP (CR�TICA_MODE(ON) isso mesmo: um OUTRO protocolo para enviar/receber
arquivos, sendo que um protocolo j� estava estabelecido CR�TICA_MODE(OFF)).

A fun��o cliente aceita comandos do usu�rio, mas os servidores (PCMJ e HTTP) trabalham de modo aut�nomo.

O programa � multithread, ou seja, (neste caso) cada fun��o (cliente, servidor PCMJ e servidor HTTP)
roda em uma thread. Al�m disso, o servidor PCMJ, ao receber cada conex�o, antes de trat�-la, abre uma
nova thread que esperar� pela pr�xima conex�o. No servidor HTTP este recurso encontra-se atualmente
desabilitado, pois em testes ocorria falha de segmenta��o quando eram feitas muitas conex�es simult�neas.


-- Dificuldades -----------------------------------------------------------------------------------------

Inicialmente, houve dificuldade em entender como as conex�es deveriam ser feitas, ou seja, quando abre,
quando fecha cada porta, qual porta abrir, como abrir, etc. Mesmo depois de definido pelo grupo como
deveriam ser feitas as conex�es, foi necess�rio mudar, pois, ao colocarmos o projeto em teste com os
outros grupos, as conex�es n�o eram realizadas. Isso porque a nossa implemeta��o esperava que se
mantivessem abertas as portas mesmo depois de recebida a informa��o solicitada. Depois de esclarecido,
pelos colegas do grupo desenvolvedor do protocolo, que cada conex�o devia ser respondida e ter sua porta
imediatamente fechada, foi poss�vel adequar o nosso projeto e faz�-lo se comunicar com os dos outros. No
entanto, houve problemas com a defini��o do protocolo, pois, embora o nosso projeto estivesse de acordo
com a defini��o, a implementa��o do servidor central, feita pelo grupo desenvolvedor do protocolo, n�o
estava. Acertados estes detalhes no nosso projeto, os programas passaram, ent�o, a se entender.

Depois, as dificuldades estavam em resolver pequenos bugs na implementa��o, cujos detalhes podem ser mais
facilmente visualizados no hist�rico do reposit�rio git. Por exemplo, a obten��o do pr�prio IP. Tanto
que manteve-se a fun��o setip para algum caso em que esta obten��o n�o funcione.

Finalmente, a dificuldade que tivemos foi em implementar um servidor e um cliente HTTP que funcionassem
corretamente para o nosso caso, pois n�o quer�amos apenas receber mensagens de texto, como em todos os
exemplos que vimos, mas arquivos bin�rios, tamb�m. Para isso era necess�rio eliminar corretamente o
cabe�alho do protocolo HTTP, o que nenhum exemplo visto fazia direito.

Obs.: a implementa��o de MD5 utilizada pode gerar warnings, dependendo do compilador, embora o �ltimo em
que o programa foi testado n�o os tenha gerado.


-- Execu��o ---------------------------------------------------------------------------------------------

Para utilizar o programa basta seguir estes passos:

1: Utilizando um terminal de comandos, navegue at� a pasta onde se encontra o programa e digite make.

2: Digite ./executeme

3: Dentro do programa, digite help, para obter a lista de comandos

4: Seja feliz!

Obs.1: � necess�rio conhecer o IP de pelo menos um usu�rio que esteja on-line para come�ar a se comunicar.
Este usu�rio on-line deve retornar, ao ser solicitado, a lista de IP's dos usu�rios que est�o conectados
com ele. Mas se nenhum usu�rio estiver on-line, � poss�vel se comunicar consigo mesmo e baixar seus
pr�prios arquivos, s� para testar (ou se divertir).

Obs.2: o programa deve mostrar no t�tulo da janela do terminal o seu IP. Caso n�o esteja correto, digite
setip <IP>, por exemplo: "setip 123.4.5.6".


---------------------------------------------------------------------------------------------------------
