BIN := my_program
OBJ := *.o
SRC := *.c

CC := gcc
CFLAGS := -g
#LDFLAGS := -lncurses

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
		$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

%.o: %.c
		$(CC) $^ $(CFLAGS) -c

clean_all: 
		rm -rf my_program *.o

clean_o:
		rm -rf *.o

clean_mp:
		rm -rf my_program
