
#pragma once

#ifndef DEBUGGING

#include <iostream>

// #define _LOGSTATEMENTS // uncomment for debug statements
#ifdef _LOGSTATEMENTS
#define LOG(stuff) std::cout << (stuff);
#else
#define LOG(stuff)
#endif

#endif