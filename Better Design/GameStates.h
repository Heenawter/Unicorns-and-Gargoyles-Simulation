#include <exception>

class RanOutOfCardsException : public std::exception
{
    public:
        virtual const char *what() const throw()
        {
            return "Ran out of cards.";
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

