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

class ActionCardException_Draw : public std::exception
{
public:
    Player* triggeringPlayer;

    ActionCardException_Draw(Player *triggeringPlayer)
    {
        this->triggeringPlayer = triggeringPlayer;
    }

    virtual const char *what() const throw()
    {
        return "Action Card -- Draw.";
    }
};

class ActionCardException_Reverse : public std::exception
{
public:
    Player *triggeringPlayer;

    ActionCardException_Reverse(Player *triggeringPlayer)
    {
        this->triggeringPlayer = triggeringPlayer;
    }

    virtual const char *what() const throw()
    {
        return "Action Card -- Reverse.";
    }
};

#endif