#pragma once

#ifndef CONSTANTS

#include <string>

static const int MAX_INT = 2147483647;

static const char RAN_OUT_OF_CARDS = 'c';
static const char RAN_OUT_OF_TIME = 't';
static const char SUCCESSFUL_CARD_DRAW = 's';
static const char NO_WIN = 'n';
static const char WIN = 'w';

static const char UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5,
                  APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;

static const int NUM_PLAYERS = 3;
static const int NUM_ROUNDS = 100;

static const int NUM_LINES_TO_SKIP = 24;

static const std::string DRAFT = "1";
static const std::string INPUT_FILE = "Simulations/Draft" + DRAFT +"_Stats.txt";
static const std::string OUTPT_FILE = "Simulations/Draft" + DRAFT + "_Results_v10 ("
    + std::to_string(NUM_ROUNDS) + " Rounds, "
    + std::to_string(NUM_PLAYERS) + " Players).txt";

#endif  