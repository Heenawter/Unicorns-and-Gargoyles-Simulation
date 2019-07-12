#pragma once

#ifndef CONSTANTS

#include <string>

static const unsigned SEED = 1500;

static const int MAX_INT = 2147483647;

static const char UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5,
                  APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;
static const char ACTION_CARD_DISCARD = 12;
static const char ACTION_CARD_DRAW = 13;
static const char ACTION_CARD_SPRING_CLEANING = 14; 
static const char ACTION_CARD_REVERSE = 15;

static const int NUM_PLAYERS = 6;
static const int NUM_ROUNDS = 1;

static const int NUM_LINES_TO_SKIP = 20;
static const int NUM_GOALS = 8;

static const std::string DRAFT = "1";
static const std::string INPUT_FILE = "Simulations/Draft" + DRAFT +"_Stats.txt";
static const std::string OUTPT_FILE = "Simulations/Draft" + DRAFT + "_Results ("
    + std::to_string(NUM_PLAYERS) + " Players, "
    + std::to_string(NUM_ROUNDS) + " Rounds)"
    + "_v1.txt";


// GAME STATE CONSTANTS
static const char NO_NEW_CARD = 'X';
static const char WIN = 'W';
static const char RAN_OUT_OF_CARDS = 'R';

#endif  