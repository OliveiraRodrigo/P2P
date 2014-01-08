Universidade Federal de Pelotas
Ciência da Computação
Redes de Computadores 1
Trabalho 1: Compartilhamento P2P

Lidiane Costa, Maureen Souza, Rodrigo Oliveira

=========================================================================================================


-- Descrição --------------------------------------------------------------------------------------------

O trabalho consiste em um compartilhador de arquivos usuário/usuário que utiliza um protocolo próprio,
chamado de PCMJ.

O programa conta com uma função cliente, para que seja possível solicitar e receber os arquivos de
outros usuários, e uma função servidor, para enviar arquivos aos usuários que os solicitarem.

Além da função servidor, que trata os comandos recebidos dos clientes, há também uma função que roda
um servidor HTTP, pois foi definido na implementação do protocolo que o envio/recebimento dos arquivos
deve ser feito via protocolo HTTP (CRÍTICA_MODE(ON) isso mesmo: um OUTRO protocolo para enviar/receber
arquivos, sendo que um protocolo já estava estabelecido CRÍTICA_MODE(OFF)).

A função cliente aceita comandos do usuário, mas os servidores (PCMJ e HTTP) trabalham de modo autônomo.

O programa é multithread, ou seja, (neste caso) cada função (cliente, servidor PCMJ e servidor HTTP)
roda em uma thread. Além disso, o servidor PCMJ, ao receber cada conexão, antes de tratá-la, abre uma
nova thread que esperará pela próxima conexão. No servidor HTTP este recurso encontra-se atualmente
desabilitado, pois em testes ocorria falha de segmentação quando eram feitas muitas conexões simultâneas.


-- Dificuldades -----------------------------------------------------------------------------------------

Inicialmente, houve dificuldade em entender como as conexões deveriam ser feitas, ou seja, quando abre,
quando fecha cada porta, qual porta abrir, como abrir, etc. Mesmo depois de definido pelo grupo como
deveriam ser feitas as conexões, foi necessário mudar, pois, ao colocarmos o projeto em teste com os
outros grupos, as conexões não eram realizadas. Isso porque a nossa implemetação esperava que se
mantivessem abertas as portas mesmo depois de recebida a informação solicitada. Depois de esclarecido,
pelos colegas do grupo desenvolvedor do protocolo, que cada conexão devia ser respondida e ter sua porta
imediatamente fechada, foi possível adequar o nosso projeto e fazê-lo se comunicar com os dos outros. No
entanto, houve problemas com a definição do protocolo, pois, embora o nosso projeto estivesse de acordo
com a definição, a implementação do servidor central, feita pelo grupo desenvolvedor do protocolo, não
estava. Acertados estes detalhes no nosso projeto, os programas passaram, então, a se entender.

Depois, as dificuldades estavam em resolver pequenos bugs na implementação, cujos detalhes podem ser mais
facilmente visualizados no histórico do repositório git. Por exemplo, a obtenção do próprio IP. Tanto
que manteve-se a função setip para algum caso em que esta obtenção não funcione.

Finalmente, a dificuldade que tivemos foi em implementar um servidor e um cliente HTTP que funcionassem
corretamente para o nosso caso, pois não queríamos apenas receber mensagens de texto, como em todos os
exemplos que vimos, mas arquivos binários, também. Para isso era necessário eliminar corretamente o
cabeçalho do protocolo HTTP, o que nenhum exemplo visto fazia direito.


-- Execução ---------------------------------------------------------------------------------------------

Para utilizar o programa basta seguir estes passos:

1: Utilizando um terminal de comandos, navegue até a pasta onde se encontra o programa e digite make.

2: Digite ./p2p

3: Dentro do programa, digite help, para obter a lista de comandos

4: Seja feliz!

Obs.1: é necessário conhecer o IP de pelo menos um usuário que esteja on-line para começar a se comunicar.
Este usuário on-line deve retornar, ao ser solicitado, a lista de IP's dos usuários que estão conectados
com ele. Mas se nenhum usuário estiver on-line, é possível se comunicar consigo mesmo e baixar seus
próprios arquivos, só para testar (ou se divertir).

Obs.2: o programa deve mostrar no título da janela do terminal o seu IP. Caso não esteja correto, digite
setip <IP>, por exemplo: "setip 123.4.5.6".


---------------------------------------------------------------------------------------------------------
