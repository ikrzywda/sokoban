$(CC) = gcc
$(CFLAGS) = `pkg-config --cflags -gtk+-3.0`
$(LIBS) = `pkg-config --libs -gtk+-3.0`

all: bin/main.o bin/sobokan/%.o
	$(CC) -o target/sobokan $(CFLAGS) $(LIBS) sobokan/%.o main.o

bin/main.o: src/main.c
	$(CC) -o *.o $(CFLAGS) $(LIBS) sobokan/%.o main.o
