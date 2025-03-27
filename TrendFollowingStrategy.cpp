#include "TrendFollowingStrategy.h"
#include "Utils.h"
#include <iostream>

TrendFollowingStrategy::TrendFollowingStrategy() : Strategy(), shortMovingAverageWindow(0), longMovingAverageWindow(0)
{
}

TrendFollowingStrategy::TrendFollowingStrategy(const std::string &name, int shortWindow, int longWindow) : Strategy(name), shortMovingAverageWindow(shortWindow), longMovingAverageWindow(longWindow)
{
}

Action TrendFollowingStrategy::decideAction(Market *market, int index, double currentHolding) const
{
    double MA_short = calculateMovingAverage(market, index, shortMovingAverageWindow);
    double MA_long = calculateMovingAverage(market, index, longMovingAverageWindow);
    if (MA_short > MA_long)
    {
        if (currentHolding)
            return HOLD;
        return BUY;
    }
    if (MA_short < MA_long)
    {
        if (!currentHolding)
            return HOLD;
        return SELL;
    }
    return HOLD;
}

TrendFollowingStrategy **TrendFollowingStrategy::generateStrategySet(const string &baseName, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow)
{
    int num_ShortWindow = (maxShortWindow - minShortWindow) / stepShortWindow + 1;
    int num_LongWindow = (maxLongWindow - minLongWindow) / stepLongWindow + 1;
    int num = num_LongWindow * num_ShortWindow;
    TrendFollowingStrategy **StrategySet = new TrendFollowingStrategy *[num];
    int count = 0;
    for (int curr_shortwindow = minShortWindow; curr_shortwindow <= maxShortWindow; curr_shortwindow += stepShortWindow)
    {
        for (int curr_longwindow = minLongWindow; curr_longwindow <= maxLongWindow; curr_longwindow += stepLongWindow)
        {
            StrategySet[count++] = new TrendFollowingStrategy(baseName + "_" + to_string(curr_shortwindow) + "_" + to_string(curr_longwindow), curr_shortwindow, curr_longwindow);
            
        }
    }
    return StrategySet;
}
