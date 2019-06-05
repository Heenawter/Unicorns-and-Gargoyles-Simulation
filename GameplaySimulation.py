import Player
import Decks

if __name__ == "__main__":
    deck = Decks.Deck()
    deck.shuffle_deck()

    goals = Decks.Goals()
    current_goal = goals.current_goal()


    players = []
    # build the player list
    for i in range(3):
        players.append(Player.Player())

    win = False
    while not win: 
        player_num = 0
        for player in players:      
            player_num += 1
            win = player.take_turn(deck, current_goal)
            if win:
                print("Player", player_num, "wins!")
                print("Goal: ", current_goal)
                player.print_cards()
                break

    # player = Player(test_deck)

    # goal = "[ ][*][ ]"
    # hand_1 = ["rotate right", "unicorn", "append 2"]
    # solution_1 = ["unicorn", "append 2", "rotate left"]

    # player.current_hand = hand_1
    # player.find_best_hand(goal)
    # print(player.current_hand)
