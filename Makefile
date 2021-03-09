FLAGS = -Wall -g 
SDL = -lSDL2 -lSDL2_ttf -lSDL2_image -lcurses

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

all : ./bin/debug

./$(BINDIR)/debug : ./$(OBJDIR)/main.o ./$(OBJDIR)/Game.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/character.o
	g++ $(FLAGS) -o $@ $^ $(SDL)

./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp ./$(SRCDIR)/game/Terrain.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Game.o : ./$(SRCDIR)/game/Game.cpp ./$(SRCDIR)/game/Game.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/character.o : ./$(SRCDIR)/game/Pacman.cpp ./$(SRCDIR)/game/Ghost.cpp ./$(SRCDIR)/game/Pacman.h ./$(SRCDIR)/game/Ghost.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Terrrain.o : ./$(SRCDIR)/game/Terrain.cpp ./$(SRCDIR)/game/Terrain.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

#Clean

clean:
	@rm obj/* bin/* data/*
