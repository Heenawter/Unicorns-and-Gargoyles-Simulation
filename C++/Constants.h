#pragma once

#ifndef CONSTANTS

static const char RAN_OUT_OF_CARDS = 'c';
static const char NO_WIN = 'n';
static const char WIN = 'w';
static const char RAN_OUT_OF_TIME = 't';

static const char UNICORN = 0, GARGOYLE = 1, REVERSE = 2, ROTATE_R = 3, ROTATE_L = 4, DOUBLE = 5,
                  APPEND_1 = 6, APPEND_2 = 7, APPEND_3 = 8, REMOVE_1 = 9, REMOVE_2 = 10, REMOVE_3 = 11;

static const int NUM_PLAYERS = 2;
static const int NUM_ROUNDS = 2;
static const int NUM_CARDS = 8;

static const int NUM_LINES_TO_SKIP = 24;

static const char *INPUT_FILE = "Draft1_Stats.txt";
static const char *OUTPT_FILE = "Draft1_Results.txt";

#endif