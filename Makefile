FLAGS = -Wall -g
SDL =-lcurses
#-lSDL2 -lSDL2_ttf -lSDL2_image

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

all : ./bin/debug

./$(BINDIR)/debug : ./$(OBJDIR)/main.o ./$(OBJDIR)/Point.o ./$(OBJDIR)/Game.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/Pacman.o
	g++ $(FLAGS) -o $@ $^ $(SDL)

./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp ./$(SRCDIR)/game/Game.h 
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Game.o : ./$(SRCDIR)/game/Game.cpp ./$(SRCDIR)/game/Game.h ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Pacman.o
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/character.o : ./$(SRCDIR)/game/Ghost.cpp ./$(SRCDIR)/game/Pacman.h ./$(SRCDIR)/game/Ghost.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Pacman.o : ./$(SRCDIR)/game/Pacman.cpp ./$(SRCDIR)/game/Pacman.cpp
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Terrrain.o : ./$(SRCDIR)/game/Terrain.cpp ./$(SRCDIR)/game/Terrain.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Point.o : ./$(SRCDIR)/game/Point.cpp ./$(SRCDIR)/game/Point.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

#Clean

clean:
	@rm obj/* bin/* data/*
