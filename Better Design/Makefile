
all: main.exe 
	.\main.exe

main.exe: Main.o Game.o Deck.o Player.o Cards.o Hand.o Player_Aggressive.o Player_Random.o
	g++ -std=c++11 -o main.exe Main.o Game.o Deck.o Player.o Cards.o Hand.o Player_Aggressive.o Player_Random.o

Main.o: Main.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Main.cpp

Game.o: Game.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Game.cpp

Deck.o: Deck.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Deck.cpp

Player.o: Player.cpp Debugging.h  Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Player.cpp

Player_Aggressive.o: Player_Aggressive.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Player_Aggressive.cpp

Player_Random.o: Player_Random.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Player_Random.cpp

Cards.o: Cards.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Cards.cpp

Hand.o: Hand.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h Player_Random.h
	g++ -std=c++11 -c Hand.cpp

clean: 
	del *.exe
	del *.o 