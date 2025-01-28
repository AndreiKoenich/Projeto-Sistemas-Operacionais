# SISTEMAS OPERACIONAIS II

# Andrei Pochmann Koenich - Matrícula 00308680

# Makefile

# Flags de compilação e de diretórios
FLAGS := -g -fsanitize=address -Wall -o

CLIENT_DIR := Client
CLIENT_SRC := $(CLIENT_DIR)/myClient.cpp \
              $(CLIENT_DIR)/clientCommands.cpp \
              $(CLIENT_DIR)/clientUtils.cpp \
              $(CLIENT_DIR)/clientPacketSending.cpp

SERVER_DIR := Server
SERVER_SRC := $(SERVER_DIR)/myServer.cpp \
              $(SERVER_DIR)/serverUtils.cpp \
              $(SERVER_DIR)/serverPacketReceiving.cpp

# Comando para linkar os arquivos compilados e gerar o executável
link: 
	g++ $(FLAGS) myClient $(CLIENT_SRC)
	g++ $(FLAGS) myServer $(SERVER_SRC)

# Comando padrão do Makefile
.DEFAULT_GOAL := link
