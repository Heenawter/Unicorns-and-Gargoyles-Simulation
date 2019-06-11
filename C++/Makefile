
all: main.exe 
	.\main.exe

main.exe: Main.o Game.o Decks.o Player.o
	g++ -std=c++11 -o main.exe Main.o Game.o Decks.o Player.o  

Main.o: Main.cpp Game.h Player.h Decks.h Constants.h
	g++ -std=c++11 -c Main.cpp

Game.o: Game.cpp Game.h Player.h Decks.h Constants.h
	g++ -std=c++11 -c Game.cpp

Decks.o: Decks.cpp Decks.h Constants.h
	g++ -std=c++11 -c Decks.cpp

Player.o: Player.cpp Player.h Decks.h Constants.h
	g++ -std=c++11 -c Player.cpp

clean: 
	del *.exe
	del *.o 