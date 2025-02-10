# SISTEMAS OPERACIONAIS II

# Andrei Pochmann Koenich - Matrícula 00308680

# Makefile

# Variáveis de flags de compilação e de diretórios
FLAGS := -g -fsanitize=address -Wall -o

CLIENT_SRC := Client/myClient.cpp \
              Client/clientCommands.cpp \
              Client/clientUtils.cpp \
              Client/clientPacketSending.cpp \
              Client/clientPacketReceiving.cpp \
              Client/inotifyClient.cpp

SERVER_SRC := Server/myServer.cpp \
              Server/serverUtils.cpp \
              Server/serverPacketReceiving.cpp \
              Server/serverPacketSending.cpp \
              Server/serverPacketPropagation.cpp

# Comando para linkar os arquivos compilados e gerar o executável
link: 
	g++ $(FLAGS) myClient $(CLIENT_SRC)
	g++ $(FLAGS) myServer $(SERVER_SRC)

# Comando padrão do Makefile
.DEFAULT_GOAL := link
