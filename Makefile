# SISTEMAS OPERACIONAIS II

# Andrei Pochmann Koenich - Matrícula 00308680

# Makefile

# Comando para linkar os arquivos compilados e gerar o executável
link: 
	g++ -g -Wall -o myClient myClient.cpp clientCommands.cpp clientUtils.cpp
	g++ -g -Wall -o myServer myServer.cpp serverUtils.cpp
	
# Comando padrão do Makefile, executa o alvo "run" por padrão
.DEFAULT_GOAL := link
