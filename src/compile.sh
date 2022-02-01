#!/usr/bin/zsh

gcc `pkg-config --cflags --libs gtk+-3.0` -g sokoban/sokoban.c sokoban/sokoban_gui.c sokoban/game.c main.c -o out
