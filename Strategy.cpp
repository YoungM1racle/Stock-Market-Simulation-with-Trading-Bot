#include "Strategy.h"
#include <iostream>

Strategy::Strategy() : name("")
{
}

Strategy::Strategy(const string &name) : name(name)
{
}

double Strategy::calculateMovingAverage(Market *market, int index, int window) const
{
    double sum = 0.0;
    if (index - (window - 1) < 0)
    {
        for (int i = 0; i < window; i++)
            sum += market->getPrice(i);
        return double(sum / double(window));
    }
    for (int i = 0; i < window; i++)
        sum += market->getPrice(index - i);
    return double(sum / double(window));
    
}

string Strategy::getName() const
{
    return name;
}
