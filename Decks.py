import GameMechanics
from random import shuffle

class Deck:
    deck = []

    def __init__(self):
        all_cards = GameMechanics.Stats().all_cards

        for type in ["string", "action"]:
            for card in all_cards[type].keys():
                for num_cards in range(all_cards[type][card]):
                    self.deck.append(card)

    def shuffle(self):
        deck = shuffle(self.deck)

    def draw_card(self):
        return self.deck.pop(0)


class Goals:
    deck = []
    current = 0

    def __init__(self):
        goal_cards = GameMechanics.Stats().all_cards["goals"]

        for card in goal_cards.keys():
            self.deck.append(card)

        deck = shuffle(self.deck)

    def current_goal(self):
        return self.deck[self.current]

    def next_goal(self):
        self.current_goal += 1
        return self.deck[self.current]
