
all: main.exe
	.\main.exe

main.exe: Main.o Game.o Decks.o StringDeck.o Player.o 
	g++ -std=c++11 -o main.exe Main.o Game.o Decks.o StringDeck.o Player.o

Main.o: Main.cpp
	g++ -std=c++11 -c Main.cpp

Game.o: Game.cpp
	g++ -std=c++11 -c Game.cpp

Decks.o: Decks.cpp
	g++ -std=c++11 -c Decks.cpp

StringDeck.o: StringDeck.cpp
	g++ -std=c++11 -c StringDeck.cpp

Player.o: Player.cpp
	g++ -std=c++11 -c Player.cpp

clean: 
	del *.exe
	del *.o 