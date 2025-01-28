# SISTEMAS OPERACIONAIS II

# Andrei Pochmann Koenich - Matrícula 00308680

# Makefile

# Variáveis de flags de compilação e de diretórios
FLAGS := -g -fsanitize=address -Wall -o

CLIENT_DIR := Client
CLIENT_SRC := $(CLIENT_DIR)/myClient.cpp \
              $(CLIENT_DIR)/clientCommands.cpp \
              $(CLIENT_DIR)/clientUtils.cpp \
              $(CLIENT_DIR)/clientPacketSending.cpp \
              $(CLIENT_DIR)/clientPacketReceiving.cpp

SERVER_DIR := Server
SERVER_SRC := $(SERVER_DIR)/myServer.cpp \
              $(SERVER_DIR)/serverUtils.cpp \
              $(SERVER_DIR)/serverPacketReceiving.cpp \
              $(SERVER_DIR)/serverPacketSending.cpp

# Comando para linkar os arquivos compilados e gerar o executável
link: 
	g++ $(FLAGS) myClient $(CLIENT_SRC)
	g++ $(FLAGS) myServer $(SERVER_SRC)

# Comando padrão do Makefile
.DEFAULT_GOAL := link
