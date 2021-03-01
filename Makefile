OPTION = -Wall -g
#create the lib

all: ./bin/client-side ./bin/server-side


./bin/server-side: ./obj/connection.o	./obj/Server.o ./obj/mainServer.o ./obj/Room.o
	g++ -lpthread $(OPTIONS) $^ -o $@

./bin/client-side: ./obj/connection.o	./obj/Client.o ./obj/mainClient.o
	g++ -lpthread $(OPTIONS) $^ -o $@



./obj/mainClient.o: client.cpp ./src/Gateway.h
	g++ $(OPTION) $< -c -o $@

./obj/mainServer.o: server.cpp ./src/Gateway.h
	g++ $(OPTION) $< -c -o $@

./obj/Client.o: ./src/Client.cpp ./src/Gateway.h ./src/connection.h
	g++ $(OPTION) $< -c -o $@

./obj/Room.o: ./src/Room.cpp ./src/Room.h ./obj/Server.o
	g++ $(OPTION) $< -c -o $@

./obj/Server.o: ./src/Server.cpp ./src/Gateway.h ./src/connection.h
	g++ -lpthread $(OPTION) $< -c -o $@

./obj/connection.o: ./src/connection.cpp ./src/connection.h
	g++ -lpthread $(OPTION) $< -c -o $@


clean:
	rm ./obj/* ./bin/*
