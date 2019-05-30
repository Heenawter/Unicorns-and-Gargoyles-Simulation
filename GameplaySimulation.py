import Player
import Decks

if __name__ == "__main__":
    deck = Decks.Deck()
    deck.shuffle()

    goals = Decks.Goals()
    current_goal = goals.current_goal()

    player1 = Player.Player()
    # player2 = Player.Player()
    # players = [player1, player2]

    win = False
    while not win:        
        win = player1.take_turn(deck, current_goal)
        if win:
            print("Player wins!")
            print("Goal: ", current_goal)
            player1.print_cards()

    # player = Player(test_deck)

    # goal = "[ ][*][ ]"
    # hand_1 = ["rotate right", "unicorn", "append 2"]
    # solution_1 = ["unicorn", "append 2", "rotate left"]

    # player.current_hand = hand_1
    # player.find_best_hand(goal)
    # print(player.current_hand)
