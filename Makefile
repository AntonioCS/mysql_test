#https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
TARGET=mysql_test
CFLAGS=-std=c11 -Wall -Werror -g `mysql_config --include`
LDFLAGS=-L/usr/lib/x86_64-linux-gnu
LDLIBS=-lmysqlclient -lpthread -lz -lm -lrt -ldl
CC=gcc
SRC=$(wildcard ./src/*.c)
#https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
OBJ=$(subst ./src, ./src/obj, $(patsubst %.c, %.o, $(SRC)))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
$(OBJ):	$(SRC)
	$(CC) $(CFLAGS) -c -o $@ $^
clean:
	rm -f $(TARGET) $(OBJ)
