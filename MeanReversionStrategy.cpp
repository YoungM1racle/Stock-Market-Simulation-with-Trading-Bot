#include "MeanReversionStrategy.h"
#include "Utils.h"
#include <cmath>
#include <iostream>

MeanReversionStrategy::MeanReversionStrategy() : Strategy(), window(0), threshold(0)
{
}

MeanReversionStrategy::MeanReversionStrategy(const string &name, int window, int threshold) : Strategy(name), window(window), threshold(threshold)
{
}

Action MeanReversionStrategy::decideAction(Market *market, int index, double currentHolding) const
{
    double MA = calculateMovingAverage(market, index, window);
    double real_threshold = double(threshold / 100.0);
    double current_price = market->getPrice(index);
    if (current_price < MA * (1 - real_threshold))
    {
        if (currentHolding)
            return HOLD;
        return BUY;
    }
    if (current_price > MA * (1 + real_threshold))
    {
        if (!currentHolding)
            return HOLD;
        return SELL;
    }
    return HOLD;
}

MeanReversionStrategy **MeanReversionStrategy::generateStrategySet(const string &baseName, int minWindow, int maxWindow, int windowStep, int minThreshold, int maxThreshold, int thresholdStep)
{
    int num_window = (maxWindow - minWindow) / windowStep + 1;
    int num_threshold = (maxThreshold - minThreshold) / thresholdStep + 1;
    int num = num_window * num_threshold;
    MeanReversionStrategy **StrategySet = new MeanReversionStrategy *[num];
    int count = 0;
    for (int curr_window = minWindow; curr_window <= maxWindow; curr_window += windowStep)
    {
        for (int curr_threshold = minThreshold; curr_threshold <= maxThreshold; curr_threshold += thresholdStep)
        {
            StrategySet[count++] = new MeanReversionStrategy(baseName + "_" + to_string(curr_window) + "_" + to_string(curr_threshold), curr_window, curr_threshold);
        }
    }
    return StrategySet;
}