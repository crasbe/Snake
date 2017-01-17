CC = gcc
CFLAGS = -Wall -std=c99
LDFLAGS = -lSDL2main -lSDL2

# specify every .c (or later .o) file as dependency
OBJ := $(patsubst %.c,%.o,$(wildcard *.c))

# those object are the only ones to be linked
snake: LDOBJ = snake.o sdlhelper.o
snake: LDOBJ += menu_$(shell bash -c 'read -p "Menu module [c,e,l]? " menu; echo $$menu').o
snake: LDOBJ += game_$(shell bash -c 'read -p "Game module [c,e,l]? " game; echo $$game').o
snake: LDOBJ += score_$(shell bash -c 'read -p "Score module [c,e,l]? " score; echo $$score').o

# By setting all .o files from every .c file as dependency
# every .c file will be compiled by the pattern rule
snake: $(OBJ)
	$(CC) $(CFLAGS) -o snake $(LDOBJ) $(LDFLAGS)

# delete all object files and the binary
clean:
	rm snake *.o

# pattern rule to compile .c files into .o files with the same name
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
