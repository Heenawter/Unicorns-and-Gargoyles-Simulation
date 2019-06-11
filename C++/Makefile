
all: main.exe
	.\main.exe

main.exe: Main.o Game.o Decks.o Player.o 
	g++ -std=c++11 -o main.exe Main.o Game.o Decks.o Player.o

Main.o: Main.cpp
	g++ -std=c++11 -c Main.cpp

Game.o: Game.cpp
	g++ -std=c++11 -c Game.cpp

Decks.o: Decks.cpp
	g++ -std=c++11 -c Decks.cpp

Player.o: Player.cpp
	g++ -std=c++11 -c Player.cpp

clean: 
	del *.exe
	del *.o 