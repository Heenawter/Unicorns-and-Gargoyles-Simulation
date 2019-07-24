
class RanOutOfCards : public std::exception
{
    const char *what() const throw()
    {
        return "Ran out of cards.";
    }
}

class GameWon: public std::exception
{
    const char *what() const throw()
    {
        return "Game complete.";
    }
}

