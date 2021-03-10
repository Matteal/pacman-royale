FLAGS = -Wall -g
SDL =-lcurses
#-lSDL2 -lSDL2_ttf -lSDL2_image

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

OBJGAME= ./$(OBJDIR)/Game.o ./$(OBJDIR)/character.o ./$(OBJDIR)/Terrrain.o ./$(OBJDIR)/Pacman.o
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
	g++ $(FLAGS) -o $@ $^ $(SDL)

# reseau
./bin/server-side: ./obj/connection.o	./obj/Server.o ./obj/server-main.o ./obj/Room.o $(OBJGAME)
	g++ $(OPTIONS) $^ -o $@ $(DEPTHREAD) $(DEPSOCKET) $(SDL)

./bin/client-side: ./obj/connection.o	./obj/Client.o ./obj/client-main.o $(OBJGAME)
	g++ $(OPTIONS) $^ -o $@ $(DEPTHREAD) $(DEPSOCKET) $(SDL)


#compilable

#debug
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

# réseau
./obj/client-main.o: src/client-main.cpp ./src/network/Gateway.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD)

./obj/server-main.o: src/server-main.cpp ./src/network/Gateway.h
	g++ $(OPTION) $< -c -o $@

./obj/Client.o: ./src/network/Client.cpp ./src/network/Gateway.h ./src/network/connection.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD) $(DEPSOCKET)

./obj/Room.o: ./src/network/Room.cpp ./src/network/Room.h ./obj/Server.o
	g++ $(OPTION) $< -c -o $@

./obj/Server.o: ./src/network/Server.cpp ./src/network/Gateway.h ./src/network/connection.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD) $(DEPSOCKET)

./obj/connection.o: ./src/network/connection.cpp ./src/network/connection.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD)




#Clean

clean:
	@rm obj/* bin/* data/*
