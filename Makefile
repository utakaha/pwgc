CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -std=c11
LDFLAGS =

TARGET = pwgc
SRC = main.c
OBJ = $(SRC:.c=.o)

PREFIX ?= /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man/man1

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

install: $(TARGET)
	install -d $(BINDIR)
	install -m 755 $(TARGET) $(BINDIR)/

uninstall:
	rm -f $(BINDIR)/$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

test: $(TARGET)
	@chmod +x test.sh
	@./test.sh

.PHONY: all install uninstall clean test