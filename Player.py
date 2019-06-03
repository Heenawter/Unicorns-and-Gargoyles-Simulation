import GameMechanics 
import Decks
from difflib import SequenceMatcher

class Player:
    action_cards = []
    current_hand = []
    all_hands = []
    all_strings = []

    def draw_card(self, deck):
        card = deck.draw_card()
        if("comment" in card or "action" in card):
            self.action_cards.append(card)
        else:
            if(len(self.all_hands) == 0):
                self.current_hand.append(card)
                self.all_hands.append([card])
                string = self.generate_string([card])
                self.all_strings.append(string)
            else:
                self.all_strings = []
                prev_hands = self.all_hands
                new_hands = []
                for hand in prev_hands:
                    for i in range(len(hand) + 1):
                        new = hand.copy()
                        new.insert(i, card)
                        string = self.generate_string(new)
                        if(string not in self.all_strings):
                            new_hands.append(new)
                            self.all_strings.append(string)
                self.all_hands = new_hands

    def generate_string(self, hand):
        string = ""
        for card in hand:
            string = GameMechanics.Cards().play_card(card, string)
        return string

    def print_cards(self):
        print("Action cards: ", self.action_cards)
        print("Current cards: ", self.current_hand)
        print("Current string: ", self.generate_string(self.current_hand))
    
    # -- Find the hand that results in a string that has ** the most possible ** similarities
    def find_best_hand(self, goal):  
        # find the permutation that makes the string as close as possible to the goal
        # start by setting the default values using the first permutation
        if(len(self.all_hands) > 0):
            current_best_hand = self.all_hands[0]
            best_string = self.generate_string(current_best_hand)
            max_similarity = SequenceMatcher(None, current_best_hand, goal).ratio()
            # now find the maximum similarity by comparing it to the others
            for try_hand in self.all_hands:  # all possible rotations
                # if you have already found a perfect match, might as well stop
                if max_similarity == 1:
                    break;
                # otherwise, try the next permutation to see if it can be improved :)
                try_hand = list(try_hand)
                try_string = self.generate_string(try_hand)  # try based on new hand
                try_similarity = SequenceMatcher(None, try_string, goal).ratio()
                if(try_similarity > max_similarity):
                    # if try hand makes better string, try is the new best
                    current_best_hand = try_hand
                    max_similarity = try_similarity

            self.current_hand = list(current_best_hand)   # set the best as the new current
        

    def take_turn(self, deck, goal):
        self.draw_card(deck)
        self.find_best_hand(goal)

        win = False
        current_string = self.generate_string(self.current_hand)
        if(current_string == goal):
            win = True
        return win

# player = Player()
# deck = Decks.Deck()
# deck.shuffle()

# for i in range(0, 13):
#     player.draw_card(deck)

# print(player.all_strings)
