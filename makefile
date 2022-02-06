CC = gcc
GTKFLAGS = $(shell pkg-config --cflags --libs gtk+-3.0)
CFLAGS = -Wall
SRCDIR = src/sokoban
TARGET = build
ASSETS = src/assets src/levels
SOURCES = src/main.c $(SRCDIR)/utils.c $(SRCDIR)/sokoban.c \
		  $(SRCDIR)/sokoban_gui.c $(SRCDIR)/game.c
OBJ = src/main.o $(SRCDIR)/utils.o $(SRCDIR)/sokoban.o \
		  $(SRCDIR)/sokoban_gui.o $(SRCDIR)/game.o

all: builddir sokoban

builddir: $(TARGET)
	cp -r $(ASSETS) $(TARGET)

$(TARGET): 
	mkdir $(TARGET)
	mkdir $(TARGET)/saves

sokoban: $(OBJ)
	gcc $(GTKFLAGS) -o $(TARGET)/sokoban $(SOURCES)

%.o: %.c
	gcc -c $(GTKFLAGS) $(CFLAGS)
