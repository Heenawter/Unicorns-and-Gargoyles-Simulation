#pragma once

#ifndef STATES_H

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

class GameWon: public std::exception
{
    public:
        virtual const char *what() const throw()
        {
            return "Game complete.";
        }
};

#endif