class Stats:
    all_cards = {}

    def __init__(self):
        def read_card_type(deck_stats):
            all_cards = {}
            num_cards = int(deck_stats.readline())
            for card in range(num_cards):
                data = deck_stats.readline()
                card_type, number = data.split(',')
                all_cards[card_type] = int(number)
            return all_cards

        deck_stats = open("Draft1_stats.txt", "r")

        # read all the information for the string manipulation cards
        deck_stats.readline()   # garbage read of first line
        string_cards = read_card_type(deck_stats)

        # do the same thing for action cards
        deck_stats.readline()   # garbage read of empty line
        deck_stats.readline()   # garbage read of title line
        action_cards = read_card_type(deck_stats)

        # now for the goal cards
        deck_stats.readline()   # garbage read of empty line
        deck_stats.readline()   # garbage read of title line
        goal_cards = read_card_type(deck_stats)

        deck_stats.close()

        self.all_cards["string"] = string_cards
        self.all_cards["action"] = action_cards
        self.all_cards["goals"] = goal_cards

class Cards:
    def unicorn(current_string):
        current_string += "[*]"
        return current_string 

    def gargoyle(current_string):
        current_string += "[G]"
        return current_string

    def reverse_order(current_string):
        temp = []
        for char in range(0, len(current_string), 3):
            temp.append(current_string[char:char + 3])
        temp.reverse()
        current_string = ''.join(temp)
        return current_string

    def rotate_right(current_string):
        end_char = current_string[len(current_string) - 3:len(current_string)]
        other_chars = current_string[0:len(current_string) - 3]
        return end_char + other_chars

    def rotate_left(current_string):
        first_char = current_string[0:3]
        other_chars = current_string[3:len(current_string)]
        return other_chars + first_char

    def double_row(current_string):
        return current_string + current_string

    def append_one(current_string):
        current_string += "[ ]"
        return current_string

    def append_two(current_string):
        current_string += "[ ][ ]"
        return current_string

    def append_three(current_string):
        current_string += "[ ][ ][ ]"
        return current_string

    def remove_one(current_string):
        return current_string[0:len(current_string) - 3]

    def remove_two(current_string):
        return current_string[0:len(current_string) - 6]

    def remove_three(current_string):
        return current_string[0:len(current_string) - 9]

    switcher = {
            "unicorn": unicorn,
            "gargoyle": gargoyle,
            "reverse order": reverse_order,
            "rotate right": rotate_right, 
            "rotate left": rotate_left,
            "double row": double_row,
            "append 1": append_one,
            "append 2": append_two,
            "append 3": append_three, 
            "remove 1": remove_one,
            "remove 2": remove_two,
            "remove 3": remove_three, 
        }

    def play_card(self, card, current_string):
        func = self.switcher.get(card)
        return func(current_string)

# print(play_card("remove 1", "[ ][*][G][ ][ ]"))
