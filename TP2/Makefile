CC=gcc
FLAGS=-Wall -Wextra
DEST= div0 tictac tictacforever monitor

all : $(DEST)

% : %.c
	$(CC) $^ -o $@ $(FLAGS)

clean :
	rm $(DEST) pipe
