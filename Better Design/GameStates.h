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

class TooManyCardsException : public std::exception
{
public:
    virtual const char *what() const throw()
    {
        return "Too many cards in player's hand - terminate.";
    }
};

#endif