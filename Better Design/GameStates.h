#pragma once

#ifndef STATES_H

#include "Player.h"
#include <exception>

class RanOutOfCardsException : public std::exception
{
    public:
        virtual const char *what() const throw()
        {
            return "Ran out of cards.";
        }
};

class OnlyActionCardsException : public std::exception
{
    public:
        virtual const char *what() const throw()
        {
            return "There is only action cards left.";
        }
};

class GameWonException: public std::exception
{
    public:
        virtual const char *what() const throw()
        {
            return "Game complete.";
        }
};

class ActionCardException : public std::exception
{
public:
    Player* triggeringPlayer;
    char type;

    ActionCardException(Player *triggeringPlayer, char type)
    {
        this->triggeringPlayer = triggeringPlayer;
        this->type = type;
    }

    virtual const char *what() const throw()
    {
        switch (this->type)
        {
        case ACTION_CARD_DRAW:
            return "Action Card -- Draw.";
        case ACTION_CARD_REVERSE:
            return "Action Card -- Reverse.";
        default:
            return "-- ERROR --";
        }
    }

    bool isDraw() { return (this->type == ACTION_CARD_DRAW); }
    bool isReverse() { return (this->type == ACTION_CARD_REVERSE); }
};

#endif