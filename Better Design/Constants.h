#pragma once

#ifndef CONSTANTS

#include <string>

static const unsigned SEED = 1500;

static const int MAX_INT = 2147483647;

static const int NUM_PLAYERS = 10;
static const int NUM_ROUNDS = 1000;

static const int NUM_LINES_TO_SKIP = 22;
static const int NUM_GOALS = 8;

static const std::string DRAFT = "1";
static const std::string INPUT_FILE = "GameInfo/Draft" + DRAFT + ".txt";
static const std::string OUTPT_FILE = "Simulations/Draft" + DRAFT + "_Results (" + std::to_string(NUM_PLAYERS) + " Players, " + std::to_string(NUM_ROUNDS) + " Rounds)" + "_v4.txt";

// GAME STATE CONSTANTS
static const char NO_NEW_CARD = 'X';
static const char WIN = 'W';
static const char RAN_OUT_OF_CARDS = 'R';
static const char REVERSE_ORDER = 'O';

#endif