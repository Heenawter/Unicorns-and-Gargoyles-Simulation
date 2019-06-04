
all: main.exe

# main.exe: Game.cpp
# 	g++ -std=c++11 -o main.exe Game.cpp Decks.cpp StringDeck.cpp

main.exe: Game.o Decks.o StringDeck.o
	g++ -o main.exe game.o decks.o stringdeck.o

Game.o: Game.cpp
	g++ -std=c++11 -c Game.cpp

Decks.o: Decks.cpp
	g++ -std=c++11 -c Decks.cpp

StringDeck.o: StringDeck.cpp
	g++ -std=c++11 -c StringDeck.cpp

clean: 
	del *.exe
	del *.o 