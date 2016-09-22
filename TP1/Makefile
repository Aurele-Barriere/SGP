CC=gcc
FLAGS=-Wall -Wextra
DEST= partie1 partie2 prodcons prod cons

all : $(DEST)

% : %.c
	$(CC) $^ -o $@ $(FLAGS)

clean :
	rm $(DEST)
