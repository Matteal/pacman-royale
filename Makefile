OPTION = -Wall -g
SDL = #-lSDL2 -lSDL2_ttf -lSDL2_image

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


# ./$(BINDIR)/debug : ./$(OBJDIR)/main.o
# 	g++ $(FLAGS) -o $@ $^ $(SDL)
#
# ./$(OBJDIR)/main.o : ./$(SRCDIR)/main.cpp
# 	g++ $(FLAGS) -c -o $@ $< $(SDL)
#create the lib

all: ./bin/client-side ./bin/server-side


./bin/server-side: ./obj/connection.o	./obj/Server.o ./obj/mainServer.o ./obj/Room.o
	g++ $(OPTIONS) $^ -o $@ $(DEPTHREAD) $(DEPSOCKET)

./bin/client-side: ./obj/connection.o	./obj/Client.o ./obj/mainClient.o
	g++ $(OPTIONS) $^ -o $@ $(DEPTHREAD) $(DEPSOCKET)



./obj/mainClient.o: client.cpp ./src/Gateway.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD)

./obj/mainServer.o: server.cpp ./src/Gateway.h
	g++ $(OPTION) $< -c -o $@ 
 
./obj/Client.o: ./src/Client.cpp ./src/Gateway.h ./src/connection.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD) $(DEPSOCKET)

./obj/Room.o: ./src/Room.cpp ./src/Room.h ./obj/Server.o
	g++ $(OPTION) $< -c -o $@ 

./obj/Server.o: ./src/Server.cpp ./src/Gateway.h ./src/connection.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD) $(DEPSOCKET)

./obj/connection.o: ./src/connection.cpp ./src/connection.h
	g++ $(OPTION) $< -c -o $@ $(DEPTHREAD)


clean:
	rm ./obj/* ./bin/*
