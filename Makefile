FLAGS = -Wall -g
CURSES =-lcurses
SDL = -lSDL2 -lSDL2_ttf -lSDL2_image

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

all : ./bin/debug

./$(BINDIR)/debug : ./$(SRCDIR)/game/Renderer.h ./$(OBJDIR)/main.o ./$(OBJDIR)/Pacgum.o ./$(OBJDIR)/Point.o ./$(OBJDIR)/Game.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/Pacman.o ./$(OBJDIR)/Renderer.o ./$(OBJDIR)/Pathfinder.o
	g++ $(FLAGS) -o $@ $^ $(CURSES) $(SDL)

./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp ./$(SRCDIR)/game/Game.h
	g++ $(FLAGS) -c -o $@ $< $(CURSES) $(SDL)

./$(OBJDIR)/Game.o : ./$(SRCDIR)/game/Game.cpp ./$(SRCDIR)/game/Game.h ./$(OBJDIR)/Pacgum.o ./$(OBJDIR)/Point.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Pacman.o ./$(OBJDIR)/Renderer.o
	g++ $(FLAGS) -c -o $@ $< $(CURSES) $(SDL)

./$(OBJDIR)/character.o : ./$(SRCDIR)/game/Ghost.cpp ./$(SRCDIR)/game/Pacman.h ./$(SRCDIR)/game/Ghost.h ./$(OBJDIR)/Point.o
	g++ $(FLAGS) -c -o $@ $<
	
./$(OBJDIR)/Pacman.o : ./$(SRCDIR)/game/Pacman.cpp ./$(SRCDIR)/game/Pacman.cpp ./$(SRCDIR)/game/direction.h
	g++ $(FLAGS) -c -o $@ $<

./$(OBJDIR)/Terrrain.o : ./$(SRCDIR)/game/Terrain.cpp ./$(SRCDIR)/game/Terrain.h ./$(SRCDIR)/game/direction.h
	g++ $(FLAGS) -c -o $@ $<

./$(OBJDIR)/Pacgum.o : ./$(SRCDIR)/game/Pacgum.cpp ./$(SRCDIR)/game/Pacgum.h
	g++ $(FLAGS) -c -o $@ $<

./$(OBJDIR)/Point.o : ./$(SRCDIR)/game/Point.cpp ./$(SRCDIR)/game/Point.h
	g++ $(FLAGS) -c -o $@ $<

./$(OBJDIR)/Renderer.o : ./$(SRCDIR)/game/Renderer.cpp ./$(SRCDIR)/game/Renderer.h
	g++ $(FLAGS) -c -o $@ $< $(CURSES) $(SDL)

./$(OBJDIR)/Pathfinder.o : ./$(SRCDIR)/ai/Pathfinder.cpp ./$(SRCDIR)/ai/Pathfinder.h
	g++ $(FLAGS) -c -o $@ $< 

#Clean

clean:
	@rm obj/* bin/*
