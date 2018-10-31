FILES_CPP = $(wildcard ./*.cpp)
FILES_C = $(wildcard ./*.c)
OBJECT= $(FILES_CPP:.cpp=.o) $(FILES_C:.c=.o)

FLAGS_DEGUB= -O0 -ggdb
FLAGS_C99= -std=c99
FLAGS_CPP= -std=c++11
FLAGS= -Wall -pedantic #-fdiagnostics-color

LIBS= -lm


EXEC = jvm

CC99 = gcc
CCPP = g++

#comando para remover pastas
RMDIR = rm -rf
#comando para remover arquivos
RM = rm -f
CD = cd

ifeq ($(OS),Windows_NT)

EXEC := $(EXEC).exe

#comando para remover um diret�rio recursivamente
RMDIR= rd /s /q
#comando para deletar um �nico arquivo
RM = del

endif

all: object
	$(CCPP) $(OBJECT) -o $(EXEC) $(FLAGS) $(LIBS)

object:
	$(CC99) -c $(FILES_C) $(FLAGS) $(LIBS)
	$(CCPP) -c $(FILES_CPP) $(FLAGS) $(LIBS)

debug: FLAGS += $(FLAGS_DEGUB)
debug: all

clean:
	$(RM) $(EXEC) *.o *.gch *.exe
