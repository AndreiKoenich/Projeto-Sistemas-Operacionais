# SISTEMAS OPERACIONAIS II

# Andrei Pochmann Koenich - Matrícula 00308680

# Makefile

# Flags de compilação
FLAGS := -g -fsanitize=address -Wall

# Comando para linkar os arquivos compilados e gerar o executável
link: 
	g++ FLAGS -o myClient Client/myClient.cpp Client/clientCommands.cpp Client/clientUtils.cpp Client/clientPacketSending.cpp
	g++ FLAGS -o myServer Server/myServer.cpp Server/serverUtils.cpp Server/serverPacketReceiving.cpp

# Comando padrão do Makefile, executa o alvo "link" por padrão
.DEFAULT_GOAL := link
