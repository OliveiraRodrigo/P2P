
# Universidade Federal de Pelotas
# Ciencia da Computacao
# Redes de Computadores 1
# Compartilhamento P2P
# Lidiane Costa, Maureen Souza, Rodrigo Oliveira

####################################################################

socompila: compila help

compila: cliente.c servidor.c p2p.c comandos.c comandos.h

	@echo  ..............................................................
	@echo   Making...

# Gera os objetos
	gcc -c  comandos.c cliente.c servidor.c p2p.c

# Gera a biblioteca estatica 'comandos.a'
	ar rcs comandos.a comandos.o

# Liga os objetos e gera o executavel utilizando a biblioteca
	gcc -o p2p cliente.o servidor.o p2p.o comandos.a -lpthread

help:
	@echo  ..............................................................
	@echo   Para executar digite: ./p2p
	@echo
	@echo   Para obter ajuda, com o programa ja aberto, digite: help
	@echo  ..............................................................

teste: compila
	@echo  ..............................................................
	@./p2p

clean:
	rm *.a *.o
