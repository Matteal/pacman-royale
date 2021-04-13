FLAGS = -Wall -g
CURSES =-lcurses
SDL = -lSDL2 -lSDL2_ttf -lSDL2_image

SRCDIR=src
HEADDIR=include
LIBDIR=build
BINDIR=bin
OBJDIR=obj

DEPTHREAD=-lpthread
DEPSOCKET=
#detecting Operating system
ifeq ($(OS),Windows_NT)     # is Windows
    DEPSOCKET:= -lws2_32
else #is Linux

endif

OBJGAME= ./$(SRCDIR)/game/Renderer.h ./$(OBJDIR)/Pacgum.o ./$(OBJDIR)/Point.o ./$(OBJDIR)/Game.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/Pacman.o ./$(OBJDIR)/Renderer.o
# ./$(BINDIR)/debug : ./$(OBJDIR)/main.o
# 	g++ $(FLAGS) -o $@ $^ $(SDL)
#
# ./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp
# 	g++ $(FLAGS) -c -o $@ $< $(SDL)
#create the lib

all: ./$(BINDIR)/debug ./bin/client-side ./bin/server-side

# exécutables

# debug
./$(BINDIR)/debug : ./$(OBJDIR)/main.o $(OBJGAME)
	g++ $(FLAGS) $^ -o $@ $(CURSES) $(SDL)

# reseau
./bin/server-side: ./obj/connection.o	./obj/Server.o ./obj/server-main.o ./obj/Room.o $(OBJGAME)
	g++ $(FLAGS) $^ -o $@ $(DEPTHREAD) $(DEPSOCKET) $(CURSES) $(SDL)

./bin/client-side: ./obj/connection.o	./obj/Client.o ./obj/client-main.o $(OBJGAME)
	g++ $(FLAGS) $^ -o $@ $(DEPTHREAD) $(DEPSOCKET) $(CURSES) $(SDL)


#compilable

# partie jeu
./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp ./$(SRCDIR)/game/Game.h
	g++ $(FLAGS) -c -o $@ $< $(CURSES) $(SDL)

./$(OBJDIR)/Game.o : ./$(SRCDIR)/game/Game.cpp ./$(SRCDIR)/game/Game.h ./$(SRCDIR)/game/Pacgum.h ./$(SRCDIR)/game/Point.h ./$(SRCDIR)/game/Terrain.h ./$(SRCDIR)/game/Character.h ./$(SRCDIR)/game/Pacman.h ./$(SRCDIR)/game/Renderer.h
	g++ $(FLAGS) -c -o $@ $< $(CURSES) $(SDL)

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

# partie réseau
./obj/client-main.o: src/client-main.cpp ./src/network/Gateway.h
	g++ $(FLAGS) $< -c -o $@ $(DEPTHREAD)

./obj/server-main.o: src/server-main.cpp ./src/network/Gateway.h
	g++ $(FLAGS) $< -c -o $@

./obj/Client.o: ./src/network/Client.cpp ./src/network/Gateway.h ./src/network/connection.h
	g++ $(FLAGS) $< -c -o $@ $(DEPTHREAD) $(DEPSOCKET)

./obj/Room.o: ./src/network/Room.cpp ./src/network/Room.h ./src/network/Gateway.h
	g++ $(FLAGS) $< -c -o $@

./obj/Server.o: ./src/network/Server.cpp ./src/network/Gateway.h ./src/network/connection.h
	g++ $(FLAGS) $< -c -o $@ $(DEPTHREAD) $(DEPSOCKET)

./obj/connection.o: ./src/network/connection.cpp ./src/network/connection.h
	g++ $(FLAGS) $< -c -o $@ $(DEPTHREAD)




#Clean

clean:
	@rm obj/* bin/*
