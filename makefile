LIBS=$(shell pkg-config --libs libcurl)
CFLAGS=$(shell pkg-config --cflags libcurl)
BINNAME=libero
RM=rm -rf

all: main clean

debug: main-debug clean

objects:
	$(CC) -c *.c $(CFLAGS)

objects-debug:
	$(CC) -c *.c -Wall $(CFLAGS) -D_DEBUG=1 -Wshadow -Wstrict-overflow -Wtype-limits -fno-strict-aliasing -Wformat -Wformat-security -g

main: objects
	$(CC) -o $(BINNAME) *.o $(LIBS)

main-debug: objects-debug
	$(CC) -o $(BINNAME) *.o $(LIBS)

clean:
	$(RM) *.o
