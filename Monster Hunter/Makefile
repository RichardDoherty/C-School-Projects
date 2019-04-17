
main:		main.cpp game.o event.o room.o wumpus.o pit.o bats.o gold.o player.o
		g++ main.cpp game.o event.o room.o wumpus.o pit.o bats.o gold.o player.o -o main

game.o: 	game.hpp game.cpp
		g++ -c game.cpp

event.o:	event.hpp event.cpp
		g++ -c event.cpp

room.o:		room.hpp room.cpp
		g++ -c room.cpp

wumpus.o:	wumpus.hpp wumpus.cpp
		g++ -c wumpus.cpp

pit.o:		pit.hpp pit.cpp
		g++ -c pit.cpp

bats.o: 	bats.hpp bats.cpp
		g++ -c bats.cpp 

gold.o:		gold.hpp gold.cpp
		g++ -c gold.cpp

player.o:	player.hpp player.cpp
		g++ -c player.cpp
clean: 
		rm -f *.o main
debugg:	
		g++ main.cpp game.o event.o room.o wumpus.o pit.o bats.o gold.o player.o -o main -g
