
#pragma once

#ifndef DEBUGGING

#include <iostream>

// #define _LOGSTATEMENTS // uncomment for debug statements
#ifdef _LOGSTATEMENTS
    #define LOG(stuff) std::cout << (stuff);
    #else
    #define LOG(stuff)
#endif

// #define _TESTSTATEMENTS // uncomment for TEST statements
#ifdef _TESTSTATEMENTS
    #define TEST(stuff) std::cout << (stuff);
    #else
    #define TEST(stuff)
#endif


// #define _DEBUGSTATEMENTS // uncomment for DEBUG statements
#ifdef _DEBUGSTATEMENTS
    #define DEBUG(stuff) std::cout << (stuff);
    #else
    #define DEBUG(stuff)
#endif

// #define _ANTIBUGSTATEMENTS // uncomment for ANTI/BUG statements
#ifdef _ANTIBUGSTATEMENTS
    class InvalidGameException : public std::exception
    {
    public:
        virtual const char *what() const throw()
        {
            return "There are more or less than 72 cards in play - terminate.";
        }
    };

    #define ANTIBUG(stuff) std::cout << (stuff);
    #else
    #define ANTIBUG(stuff)
#endif

#endif