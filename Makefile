FLAGS = -Wall -g 
SDL = -lSDL2 -lSDL2_ttf -lSDL2_image -lcurses

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

all : ./bin/debug

./$(BINDIR)/debug : ./$(OBJDIR)/main.o ./$(OBJDIR)/Terrrain.o
	g++ $(FLAGS) -o $@ $^ $(SDL)

./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp ./$(SRCDIR)/Terrain.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)

#Modules

./$(OBJDIR)/Terrrain.o : ./$(SRCDIR)/Terrain.cpp ./$(SRCDIR)/Terrain.h
	g++ $(FLAGS) -c -o $@ $< $(SDL)


#Clean

clean:
	@rm out/* bin/* data/*
