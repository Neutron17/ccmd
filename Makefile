PREFIX = /usr/local/bin
CC = gcc


all: build

build:
	$(CC) cat.c -o ncat
	$(CC) ls.c -o nls

install: build
	cp ncat $(PREFIX)
	cp nls $(PREFIX)

clean:
	rm ncat nls
