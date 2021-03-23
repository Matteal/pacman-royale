FLAGS = -Wall -g
SDL =-lcurses
#-lSDL2 -lSDL2_ttf -lSDL2_image

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

all : ./bin/debug

./$(BINDIR)/debug : ./$(SRCDIR)/game/Renderer.h ./$(OBJDIR)/main.o ./$(OBJDIR)/Pacgum.o ./$(OBJDIR)/Point.o ./$(OBJDIR)/Game.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/Pacman.o ./$(OBJDIR)/Renderer.o
	g++ $(FLAGS) -o $@ $^ $(SDL)

./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp ./$(SRCDIR)/game/Game.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Game.o : ./$(SRCDIR)/game/Game.cpp ./$(SRCDIR)/game/Game.h ./$(OBJDIR)/Pacgum.o ./$(OBJDIR)/Point.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Pacman.o ./$(OBJDIR)/Renderer.o
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/character.o : ./$(SRCDIR)/game/Ghost.cpp ./$(SRCDIR)/game/Pacman.h ./$(SRCDIR)/game/Ghost.h ./$(OBJDIR)/Point.o
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Pacman.o : ./$(SRCDIR)/game/Pacman.cpp ./$(SRCDIR)/game/Pacman.cpp
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Terrrain.o : ./$(SRCDIR)/game/Terrain.cpp ./$(SRCDIR)/game/Terrain.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Pacgum.o : ./$(SRCDIR)/game/Pacgum.cpp ./$(SRCDIR)/game/Pacgum.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Point.o : ./$(SRCDIR)/game/Point.cpp ./$(SRCDIR)/game/Point.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

./$(OBJDIR)/Renderer.o : ./$(SRCDIR)/game/Renderer.cpp ./$(SRCDIR)/game/Renderer.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

#Clean

clean:
	@rm obj/* bin/* data/*
