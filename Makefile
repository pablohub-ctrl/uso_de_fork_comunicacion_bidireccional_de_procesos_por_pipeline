
# Parte compilador
CC = gcc
P_DEF = -D DISABLE_SIMPLE_LOGGER
# flags
CFLAGS = -g -Wall
LPNG = `pkg-config libpng --libs`

# directorios
SRC = src/
# ejec
RUN = ./pipeline

RM = rm

all: pipeline.o pconvolucion.o prectificacion.o ppooling.o pclasificador.o p_writing.o run

pipeline.o: 
	@$(CC) $(P_DEF) $(CFLAGS) $(SRC)pipeline.c -o $(SRC)pipeline $(LPNG) 

pconvolucion.o: 
	@$(CC) $(SRC)pconvolucion.c -o $(SRC)pconvolucion

prectificacion.o: 
	@$(CC) $(SRC)prectificacion.c -o $(SRC)prectificacion

ppooling.o: 
	@$(CC) $(SRC)ppooling.c -o $(SRC)ppooling

pclasificador.o: 
	@$(CC) $(SRC)pclasificador.c -o $(SRC)pclasificador	

p_writing.o: 
	@$(CC) $(P_DEF) $(CFLAGS) $(SRC)p_writing.c -o $(SRC)p_writing $(LPNG) 
	
run:
	./$(SRC)pipeline -c 6 -m filtro.txt -n 0.5 -p 50 -b


clean: clean1 clean2

clean1:
		@$(RM) $(SRC)pipeline $(SRC)pconvolucion $(SRC)prectificacion $(SRC)ppooling $(SRC)pclasificador $(SRC)p_writing

clean2:
		@$(RM) *.png