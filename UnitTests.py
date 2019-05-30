import unittest
import Player
import Decks
import GameMechanics


class TestGameMechanics(unittest.TestCase):
    rules = GameMechanics.Cards()

    def test_unicorn(self):
        test_string_1 = ""
        test_string_2 = "[*]"
        test_string_3 = "[ ][ ][ ]"

        expected_string_1 = "[*]"
        expected_string_2 = "[*][*]"
        expected_string_3 = "[ ][ ][ ][*]"

        actual_string_1 = self.rules.play_card("unicorn", test_string_1)
        actual_string_2 = self.rules.play_card("unicorn", test_string_2)
        actual_string_3 = self.rules.play_card("unicorn", test_string_3)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)

    def test_gargoyle(self):
        test_string_1 = ""
        test_string_2 = "[G]"
        test_string_3 = "[ ][ ][ ]"

        expected_string_1 = "[G]"
        expected_string_2 = "[G][G]"
        expected_string_3 = "[ ][ ][ ][G]"

        actual_string_1 = self.rules.play_card("gargoyle", test_string_1)
        actual_string_2 = self.rules.play_card("gargoyle", test_string_2)
        actual_string_3 = self.rules.play_card("gargoyle", test_string_3)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)

    def test_reverse(self):
        test_string_1 = ""
        test_string_2 = "[ ][*][ ]"
        test_string_3 = "[ ][ ][*]"
        test_string_4 = "[ ][G][*][ ]"

        expected_string_1 = ""
        expected_string_2 = "[ ][*][ ]"
        expected_string_3 = "[*][ ][ ]"
        expected_string_4 = "[ ][*][G][ ]"

        actual_string_1 = self.rules.play_card("reverse order", test_string_1)
        actual_string_2 = self.rules.play_card("reverse order", test_string_2)
        actual_string_3 = self.rules.play_card("reverse order", test_string_3)
        actual_string_4 = self.rules.play_card("reverse order", test_string_4)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)
        self.assertEqual(expected_string_4, actual_string_4)

    def test_rotate_right(self):
        test_string_1 = ""
        test_string_2 = "[*]"
        test_string_3 = "[*][ ]"
        test_string_4 = "[ ][ ][*]"
        test_string_5 = "[ ][G][*][ ]"
        test_string_6 = "[ ][ ][G][*]"

        expected_string_1 = ""
        expected_string_2 = "[*]"
        expected_string_3 = "[ ][*]"
        expected_string_4 = "[*][ ][ ]"
        expected_string_5 = "[ ][ ][G][*]"
        expected_string_6 = "[*][ ][ ][G]"

        actual_string_1 = self.rules.play_card("rotate right", test_string_1)
        actual_string_2 = self.rules.play_card("rotate right", test_string_2)
        actual_string_3 = self.rules.play_card("rotate right", test_string_3)
        actual_string_4 = self.rules.play_card("rotate right", test_string_4)
        actual_string_5 = self.rules.play_card("rotate right", test_string_5)
        actual_string_6 = self.rules.play_card("rotate right", test_string_6)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)
        self.assertEqual(expected_string_4, actual_string_4)
        self.assertEqual(expected_string_5, actual_string_5)
        self.assertEqual(expected_string_6, actual_string_6)

    def test_rotate_left(self):
        test_string_1 = ""
        test_string_2 = "[*]"
        test_string_3 = "[*][ ]"
        test_string_4 = "[ ][ ][*]"
        test_string_5 = "[ ][G][*][ ]"
        test_string_6 = "[G][*][ ][ ]"

        expected_string_1 = ""
        expected_string_2 = "[*]"
        expected_string_3 = "[ ][*]"
        expected_string_4 = "[ ][*][ ]"
        expected_string_5 = "[G][*][ ][ ]"
        expected_string_6 = "[*][ ][ ][G]"

        actual_string_1 = self.rules.play_card("rotate left", test_string_1)
        actual_string_2 = self.rules.play_card("rotate left", test_string_2)
        actual_string_3 = self.rules.play_card("rotate left", test_string_3)
        actual_string_4 = self.rules.play_card("rotate left", test_string_4)
        actual_string_5 = self.rules.play_card("rotate left", test_string_5)
        actual_string_6 = self.rules.play_card("rotate left", test_string_6)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)
        self.assertEqual(expected_string_4, actual_string_4)
        self.assertEqual(expected_string_5, actual_string_5)
        self.assertEqual(expected_string_6, actual_string_6)

    def test_double_row(self):
        test_string_1 = ""
        test_string_2 = "[ ]"
        test_string_3 = "[ ][*][ ]"
        test_string_4 = "[ ][G][*][ ]"

        expected_string_1 = ""
        expected_string_2 = "[ ][ ]"
        expected_string_3 = "[ ][*][ ][ ][*][ ]"
        expected_string_4 = "[ ][G][*][ ][ ][G][*][ ]"

        actual_string_1 = self.rules.play_card("double row", test_string_1)
        actual_string_2 = self.rules.play_card("double row", test_string_2)
        actual_string_3 = self.rules.play_card("double row", test_string_3)
        actual_string_4 = self.rules.play_card("double row", test_string_4)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)
        self.assertEqual(expected_string_4, actual_string_4)

    def test_append_one(self):
        test_string_1 = ""
        test_string_2 = "[ ][*][ ]"

        expected_string_1 = "[ ]"
        expected_string_2 = "[ ][*][ ][ ]"

        actual_string_1 = self.rules.play_card("append 1", test_string_1)
        actual_string_2 = self.rules.play_card("append 1", test_string_2)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)

    def test_append_two(self):
        test_string_1 = ""
        test_string_2 = "[ ][*][ ]"

        expected_string_1 = "[ ][ ]"
        expected_string_2 = "[ ][*][ ][ ][ ]"

        actual_string_1 = self.rules.play_card("append 2", test_string_1)
        actual_string_2 = self.rules.play_card("append 2", test_string_2)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)

    def test_append_three(self):
        test_string_1 = ""
        test_string_2 = "[ ][*][ ]"

        expected_string_1 = "[ ][ ][ ]"
        expected_string_2 = "[ ][*][ ][ ][ ][ ]"

        actual_string_1 = self.rules.play_card("append 3", test_string_1)
        actual_string_2 = self.rules.play_card("append 3", test_string_2)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)

    def test_remove_one(self):
        test_string_1 = ""
        test_string_2 = "[ ][*]"
        test_string_3 = "[ ][*][G]"

        expected_string_1 = ""
        expected_string_2 = "[ ]"
        expected_string_3 = "[ ][*]"

        actual_string_1 = self.rules.play_card("remove 1", test_string_1)
        actual_string_2 = self.rules.play_card("remove 1", test_string_2)
        actual_string_3 = self.rules.play_card("remove 1", test_string_3)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)

    def test_remove_two(self):
        test_string_1 = ""
        test_string_2 = "[ ]"
        test_string_3 = "[ ][*]"
        test_string_4 = "[ ][*][G]"
        test_string_5 = "[ ][G][*][ ]"

        expected_string_1 = ""
        expected_string_2 = ""
        expected_string_3 = ""
        expected_string_4 = "[ ]"
        expected_string_5 = "[ ][G]"

        actual_string_1 = self.rules.play_card("remove 2", test_string_1)
        actual_string_2 = self.rules.play_card("remove 2", test_string_2)
        actual_string_3 = self.rules.play_card("remove 2", test_string_3)
        actual_string_4 = self.rules.play_card("remove 2", test_string_4)
        actual_string_5 = self.rules.play_card("remove 2", test_string_5)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)
        self.assertEqual(expected_string_4, actual_string_4)
        self.assertEqual(expected_string_5, actual_string_5)

    def test_remove_three(self):
        test_string_1 = ""
        test_string_2 = "[ ]"
        test_string_3 = "[ ][*][ ]"
        test_string_4 = "[ ][*][ ][G][*]"

        expected_string_1 = ""
        expected_string_2 = ""
        expected_string_3 = ""
        expected_string_4 = "[ ][*]"

        actual_string_1 = self.rules.play_card("remove 3", test_string_1)
        actual_string_2 = self.rules.play_card("remove 3", test_string_2)
        actual_string_3 = self.rules.play_card("remove 3", test_string_3)
        actual_string_4 = self.rules.play_card("remove 3", test_string_4)

        self.assertEqual(expected_string_1, actual_string_1)
        self.assertEqual(expected_string_2, actual_string_2)
        self.assertEqual(expected_string_3, actual_string_3)
        self.assertEqual(expected_string_4, actual_string_4)


class TestStringGeneration(unittest.TestCase):
    deck = Decks.Deck()
    deck.shuffle()
    player = Player.Player(deck)

    def test_append(self):
        goal = "[ ][ ][ ]"

        hand_1 = ["append 3"]
        hand_2 = ["append 1", "append 2"]
        hand_3 = ["append 1", "append 1", "append 1"]

        result_1 = self.player.generate_string(hand_1)
        result_2 = self.player.generate_string(hand_2)
        result_3 = self.player.generate_string(hand_3)

        self.assertEqual(goal, result_1)
        self.assertEqual(goal, result_2)
        self.assertEqual(goal, result_3)

    def test_remove(self):
        goal = "[ ]"

        hand_1 = ["append 3", "remove 2"]
        hand_2 = ["append 2", "remove 1"]
        hand_3 = ["append 3", "append 1", "remove 3"]

        result_1 = self.player.generate_string(hand_1)
        result_2 = self.player.generate_string(hand_2)
        result_3 = self.player.generate_string(hand_3)

        self.assertEqual(goal, result_1)
        self.assertEqual(goal, result_2)
        self.assertEqual(goal, result_3)

    def test_double(self):
        goal = "[ ][ ][*][ ][ ][*]"
        hand_1 = ["append 2", "unicorn", "double row"]
        hand_2 = ["append 1", "double row", "unicorn", "double row"]

        result_1 = self.player.generate_string(hand_1)
        result_2 = self.player.generate_string(hand_2)

        self.assertEqual(goal, result_1)
        self.assertEqual(goal, result_2)

    def test_single_movements(self):
        goal = "[*][ ][ ]"

        hand_1 = ["append 2", "unicorn", "reverse order"]
        hand_2 = ["append 3", "remove 1", "unicorn", "reverse order"]
        hand_3 = ["append 2", "unicorn", "rotate right"]
        hand_4 = ["append 1", "unicorn", "append 1", "rotate left"]

        result_1 = self.player.generate_string(hand_1)
        result_2 = self.player.generate_string(hand_2)
        result_3 = self.player.generate_string(hand_3)
        result_4 = self.player.generate_string(hand_4)

        self.assertEqual(goal, result_1)
        self.assertEqual(goal, result_2)
        self.assertEqual(goal, result_3)
        self.assertEqual(goal, result_4)

    def test_multiple_movements(self):
        goal = "[*][ ][ ]"

        hand_1 = ["append 2", "unicorn", "reverse order",
                  "reverse order", "reverse order"]
        hand_2 = ["append 3", "reverse order",
                  "remove 1", "unicorn", "reverse order"]
        hand_3 = ["append 2", "unicorn", "rotate left",
                  "rotate right", "rotate right"]
        hand_4 = ["append 1", "unicorn", "append 1",
                  "reverse order", "rotate left"]
        hand_5 = ["append 3", "rotate right", "remove 1",
                  "rotate left", "unicorn", "reverse order"]

        result_1 = self.player.generate_string(hand_1)
        result_2 = self.player.generate_string(hand_2)
        result_3 = self.player.generate_string(hand_3)
        result_4 = self.player.generate_string(hand_4)
        result_5 = self.player.generate_string(hand_5)

        self.assertEqual(goal, result_1)
        self.assertEqual(goal, result_2)
        self.assertEqual(goal, result_3)
        self.assertEqual(goal, result_4)
        self.assertEqual(goal, result_5)

    def test_complicated_strings(self):
        goal_1 = "[ ][G][*][ ]"

        hand_1 = ["append 2", "gargoyle", "unicorn", "rotate left"]
        hand_2 = ["gargoyle", "unicorn", "append 2", "rotate right"]
        hand_3 = ["append 3", "unicorn", "gargoyle",
                  "rotate left", "rotate left", "remove 1"]
        hand_4 = ["append 1", "gargoyle", "double row",
                  "remove 2", "unicorn", "append 1"]

        result_1 = self.player.generate_string(hand_1)
        result_2 = self.player.generate_string(hand_2)
        result_3 = self.player.generate_string(hand_3)
        result_4 = self.player.generate_string(hand_4)

        goal_2 = "[ ][ ][*][G][*][ ]"

        hand_5 = ["gargoyle", "unicorn", "append 3",
                  "reverse order", "unicorn", "rotate left"]
        hand_6 = ["append 1", "unicorn", "double row", "remove 1",
                  "rotate right", "gargoyle", "unicorn", "append 1"]

        result_5 = self.player.generate_string(hand_5)
        result_6 = self.player.generate_string(hand_6)

        self.assertEqual(goal_2, result_5)
        self.assertEqual(goal_2, result_6)


class TestBestHand(unittest.TestCase):
    deck = Decks.Deck()
    deck.shuffle()
    player = Player.Player(deck)

    def test_already_at_goal(self):
        goal = "[ ][ ][ ]"

        hand_1 = ["append 3"]
        hand_2 = ["append 2", "append 1"]
        hand_3 = ["append 1", "append 1", "append 1"]
        hand_4 = ["remove 1", "append 1", "append 1", "append 1"]

        hands = [hand_1, hand_2, hand_3, hand_4]

        i = 1
        for hand in hands:
            self.player.current_hand = hand
            self.player.find_best_hand(goal)
            self.assertEqual(hand, self.player.current_hand,
                             msg="Failed on hand " + str(i))
            i += 1

    def test_all_useful_cards(self):
        goal = "[ ][*][ ]"

        hand_1 = ["rotate right", "unicorn", "append 2"]
        hand_2 = ["append 1", "append 1", "unicorn"]
        hand_3 = ["remove 1", "append 1", "append 2", "unicorn"]
        hand_4 = ["remove 3", "unicorn", "append 1", "append 1", "append 3"]

        hands = [hand_1, hand_2, hand_3, hand_4]

        i = 1
        for hand in hands:
            self.player.current_hand = hand
            self.player.find_best_hand(goal)
            solution = self.player.generate_string(self.player.current_hand)
            self.assertEqual(goal, solution, msg="Failed on hand " + str(i))
            i += 1

    def test_unnecessary_cards(self):
        goal = "[ ][*][ ]"

        hand_1 = ["rotate right", "unicorn", "append 2", "remove 1"]
        hand_2 = ["append 1", "append 1", "unicorn", "remove 2", "rotate left"]
        hand_3 = ["remove 1", "append 1", "append 2",
                  "unicorn", "rotate right", "reverse order"]
        hand_4 = ["remove 3", "unicorn", "append 1", "append 1",
                  "append 3", "reverse order", "rotate left", "remove 3"]

        hands = [hand_1, hand_2, hand_3, hand_4]

        i = 1
        for hand in hands:
            self.player.current_hand = hand
            self.player.find_best_hand(goal)
            solution = self.player.generate_string(self.player.current_hand)
            self.assertEqual(goal, solution, msg="Failed on hand " + str(i))
            i += 1

    def test_not_possible(self):
        goal = "[ ][*][ ]"

        hand_1 = ["append 2"]
        hand_2 = ["append 1", "append 1"]
        hand_3 = ["unicorn", "append 1", "double row"]

        hands = [hand_1, hand_2, hand_3]

        i = 1
        for hand in hands:
            self.player.current_hand = hand
            self.player.find_best_hand(goal)
            solution = self.player.generate_string(self.player.current_hand)
            self.assertNotEqual(goal, solution, msg="Failed on hand " + str(i))
            i += 1

    def test_complicated_goals(self):
        goal_1 = "[ ][ ][G][*][ ][ ]"
        goal_2 = "[ ][*][*][ ][*][*][G][ ]"
        goal_3 = "[*][ ][G][ ][*][ ]"

        hand_1 = ["gargoyle", "append 3", "append 2", "remove 1", "unicorn"]
        hand_2 = ["remove 2", "double row", "append 1",
                  "unicorn", "gargoyle", "unicorn", "append 3"]
        hand_3 = ["gargoyle", "rotate left", "unicorn", "append 1",
                  "double row", "rotate left", "append 1", "reverse order"]

        goals = [goal_1, goal_2, goal_3]
        hands = [hand_1, hand_2, hand_3]

        for i in range(len(hands)):
            self.player.current_hand = hands[i]
            self.player.find_best_hand(goals[i])
            solution = self.player.generate_string(self.player.current_hand)
            self.assertEqual(goals[i], solution, msg="Failed on hand " + str(i))


def suite():
    # gather all the tests from this module in a test suite.
    test_suite = unittest.TestSuite()
    test_suite.addTest(unittest.makeSuite(TestGameMechanics))
    test_suite.addTest(unittest.makeSuite(TestStringGeneration))
    test_suite.addTest(unittest.makeSuite(TestBestHand))
    return test_suite


mySuit = suite()
runner = unittest.TextTestRunner()
runner.run(mySuit)
