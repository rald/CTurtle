turtle: main.c util.h gl2d.h turtle.h
	gcc main.c -o turtle -lglfw -lGL -lX11 -lXrandr -lm -Wall -O2
	termux-elf-cleaner turtle

.PHONY: clean

clean:
	rm turtle
