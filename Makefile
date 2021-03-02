FLAGS = -Wall -g
SDL = #-lSDL2 -lSDL2_ttf -lSDL2_image

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

all : ./bin/debug

./$(BINDIR)/debug : ./$(OBJDIR)/main.o ./$(OBJDIR)/character.o
	g++ $(FLAGS) -o $@ $^ $(SDL)

./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/character.o : ./$(SRCDIR)/game/Pacman.cpp ./$(SRCDIR)/game/Ghost.cpp ./$(SRCDIR)/game/Pacman.h ./$(SRCDIR)/game/Ghost.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

clean:
	@rm out/* bin/* data/*
