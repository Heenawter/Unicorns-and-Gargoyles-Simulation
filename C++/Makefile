
all: main.exe
	.\main.exe

main.exe: Game.o Decks.o StringDeck.o GoalsDeck.o
	g++ -o main.exe Game.o Decks.o StringDeck.o GoalsDeck.o 

Game.o: Game.cpp
	g++ -std=c++11 -c Game.cpp

Decks.o: Decks.cpp
	g++ -std=c++11 -c Decks.cpp

StringDeck.o: StringDeck.cpp
	g++ -std=c++11 -c StringDeck.cpp

GoalsDeck.o: GoalsDeck.cpp
	g++ -std=c++11 -c GoalsDeck.cpp

clean: 
	del *.exe
	del *.o 