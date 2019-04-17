all: gofish

gofish: gofish.cpp card.o deck.o hand.o player.o game.o
	g++ gofish.cpp card.o deck.o hand.o player.o game.o -o gofish

card.o: Card.hpp card.cpp
	g++ -c card.cpp -o card.o

deck.o: Deck.hpp deck.cpp
	g++ -c deck.cpp -o deck.o

hand.o: Hand.hpp hand.cpp 
	g++ -c hand.cpp -o hand.o

player.o: Player.hpp player.cpp
	g++ -c player.cpp -o player.o

game.o: Game.hpp game.cpp
	g++ -c game.cpp -o game.o

clean: 
	rm -f *.o gofish 
