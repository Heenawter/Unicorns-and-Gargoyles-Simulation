
all: executable 

<<<<<<< HEAD
executable: Main.o Game.o Deck.o Player.o Cards.o Hand.o Player_Greedy.o Player_Random.o Player_Aggressive.o
	g++ -std=c++11 -o executable Main.o Game.o Deck.o Player.o Cards.o Hand.o Player_Greedy.o Player_Random.o Player_Aggressive.o
=======
main.exe: Main.o Game.o Deck.o Player.o Cards.o Hand.o Player_Greedy.o Player_Random.o Player_Aggressive.o
	g++ -std=c++11 -o main.exe Main.o Game.o Deck.o Player.o Cards.o Hand.o Player_Greedy.o Player_Random.o Player_Aggressive.o
>>>>>>> f3b237074d3a5d2d20e6bc36e1c3809893dfc6a8

Main.o: Main.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h Player_Random.h Player_Aggressive.h
	g++ -std=c++11 -c Main.cpp

Game.o: Game.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h Player_Random.h Player_Aggressive.h
	g++ -std=c++11 -c Game.cpp

Deck.o: Deck.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h Player_Random.h Player_Aggressive.h
	g++ -std=c++11 -c Deck.cpp

Player.o: Player.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h Player_Random.h Player_Aggressive.h
	g++ -std=c++11 -c Player.cpp

Player_Greedy.o: Player_Greedy.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h
	g++ -std=c++11 -c Player_Greedy.cpp

Player_Random.o: Player_Random.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Random.h
	g++ -std=c++11 -c Player_Random.cpp

Player_Aggressive.o: Player_Aggressive.cpp Debugging.h Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Aggressive.h
	g++ -std=c++11 -c Player_Aggressive.cpp

Cards.o: Cards.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h Player_Random.h Player_Aggressive.h
	g++ -std=c++11 -c Cards.cpp

Hand.o: Hand.cpp Game.h Player.h Deck.h Hand.h Cards.h Constants.h GameStates.h Player_Greedy.h Player_Random.h Player_Aggressive.h
	g++ -std=c++11 -c Hand.cpp

clean: 
	rm *.o 