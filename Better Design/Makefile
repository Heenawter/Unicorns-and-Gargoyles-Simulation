
all: main.exe 
	.\main.exe

main.exe: Main.o Game.o Deck.o Player.o Cards.o
	g++ -std=c++11 -o main.exe Main.o Game.o Deck.o Player.o Cards.o

Main.o: Main.cpp Main.h GameStates.h Game.h Player.h Deck.h Constants.h Cards.h
	g++ -std=c++11 -c Main.cpp

Game.o: Game.cpp Game.h Player.h Deck.h Constants.h Cards.h
	g++ -std=c++11 -c Game.cpp

Deck.o: Deck.cpp Deck.h Constants.h Cards.h
	g++ -std=c++11 -c Deck.cpp

Player.o: Player.cpp Player.h Deck.h Constants.h Cards.h
	g++ -std=c++11 -c Player.cpp

Cards.o: Cards.h Constants.h
	g++ -std=c++11 -c Cards.cpp

clean: 
	del *.exe
	del *.o 