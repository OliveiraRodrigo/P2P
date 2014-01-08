Universidade Federal de Pelotas
Ciencia da Computacao
Redes de Computadores 1
Trabalho 1: Compartilhamento P2P

Lidiane Costa, Maureen Souza, Rodrigo Oliveira

=========================================================================================================


-- Descricao --------------------------------------------------------------------------------------------

O trabalho consiste em um compartilhador de arquivos usuario/usuario que utiliza um protocolo proprio,
chamado de PCMJ.

O programa conta com uma funcao cliente, para que seja possivel solicitar e receber os arquivos de
outros usuarios, e uma funcao servidor, para enviar arquivos aos usuarios que os solicitarem.

Alem da funcao servidor, que trata os comandos recebidos dos clientes, ha tambem uma funcao que roda
um servidor HTTP, pois foi definido na implementacao do protocolo que o envio/recebimento dos arquivos
deve ser feito via protocolo HTTP (CRITICA_MODE(ON) isso mesmo: um OUTRO protocolo para enviar/receber
arquivos, sendo que um protocolo ja estava estabelecido CRITICA_MODE(OFF)).

A funcao cliente aceita comandos do usuario, mas os servidores (PCMJ e HTTP) trabalham de modo autonomo.

O programa e multithread, ou seja, (neste caso) cada funcao (cliente, servidor PCMJ e servidor HTTP)
roda em uma thread. Alem disso, o servidor PCMJ, ao receber cada conexao, antes de trata-la, abre uma
nova thread que esperara pela proxima conexao. No servidor HTTP este recurso encontra-se atualmente
desabilitado, pois em testes ocorria falha de segmentacao quando eram feitas muitas conexoes simultaneas.


-- Dificuldades -----------------------------------------------------------------------------------------

Inicialmente, houve dificuldade em entender como as conexoes deveriam ser feitas, ou seja, quando abre,
quando fecha cada porta, qual porta abrir, como abrir, etc. Mesmo depois de definido pelo grupo como
deveriam ser feitas as conexoes, foi necessario mudar, pois, ao colocarmos o projeto em teste com os
outros grupos, as conexoes nao eram realizadas. Isso porque a nossa implemetaCao esperava que se
mantivessem abertas as portas mesmo depois de recebida a informacao solicitada. Depois de esclarecido,
pelos colegas do grupo desenvolvedor do protocolo, que cada conexao devia ser respondida e ter sua porta
imediatamente fechada, foi possivel adequar o nosso projeto e faze-lo se comunicar com os dos outros. No
entanto, houve problemas com a definicao do protocolo, pois, embora o nosso projeto estivesse de acordo
com a definicao, a implementacao do servidor central, feita pelo grupo desenvolvedor do protocolo, nao
estava. Acertados estes detalhes no nosso projeto, os programas passaram, entao, a se entender.

Depois, as dificuldades estavam em resolver pequenos bugs na implementacao, cujos detalhes podem ser mais
facilmente visualizados no historico do repositorio git. Por exemplo, a obtencao do proprio IP. Tanto
que manteve-se a funcao setip para algum caso em que esta obtencao nao funcione.

Finalmente, a dificuldade que tivemos foi em implementar um servidor e um cliente HTTP que funcionassem
corretamente para o nosso caso, pois nao queriamos apenas receber mensagens de texto, como em todos os
exemplos que vimos, mas arquivos binarios, tambem. Para isso era necessario eliminar corretamente o
cabecalho do protocolo HTTP, o que nenhum exemplo visto fazia direito.


-- ExecuCao ---------------------------------------------------------------------------------------------

Para utilizar o programa basta seguir estes passos:

1: Utilizando um terminal de comandos, navegue ate a pasta onde se encontra o programa e digite make.

2: Digite ./p2p

3: Dentro do programa, digite help, para obter a lista de comandos

4: Seja feliz!

Obs.1: e necessario conhecer o IP de pelo menos um usuario que esteja on-line para comecar a se comunicar.
Este usuario on-line deve retornar, ao ser solicitado, a lista de IP's dos usuarios que estao conectados
com ele. Mas se nenhum usuario estiver on-line, e possivel se comunicar consigo mesmo e baixar seus
proprios arquivos, so para testar (ou se divertir).

Obs.2: o programa deve mostrar no titulo da janela do terminal o seu IP. Caso nao esteja correto, digite
setip <IP>, por exemplo: "setip 123.4.5.6".


---------------------------------------------------------------------------------------------------------
