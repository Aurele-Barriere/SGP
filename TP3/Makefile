CC=gcc
FLAGS=-Wall -Wextra -lpthread
DEST= matmult

all : $(DEST)

% : %.c
	$(CC) $^ -o $@ $(FLAGS)

clean :
	rm $(DEST)
