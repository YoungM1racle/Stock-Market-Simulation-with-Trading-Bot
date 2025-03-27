#include "WeightedTrendFollowingStrategy.h"
#include "Utils.h"
#include <cmath>

WeightedTrendFollowingStrategy::WeightedTrendFollowingStrategy() : TrendFollowingStrategy()
{
}

WeightedTrendFollowingStrategy::WeightedTrendFollowingStrategy(const string &name, int shortWindow, int longWindow) : TrendFollowingStrategy(name, shortWindow, longWindow)
{
}

double WeightedTrendFollowingStrategy::calculateExponentialWeight(int index) const
{
    double weight = 1.0;
    for (int i = 0; i < index; i++)
    {
        weight *= 1.1;
    }
    return weight;
}

double WeightedTrendFollowingStrategy::calculateMovingAverage(Market *market, int index, int window) const
{
    double weight_sum = 0.0;
    double sum = 0.0;
    if (index - (window - 1) < 0)
    {
        for (int i = 0; i < window; i++)
        {
            weight_sum += calculateExponentialWeight(i);
            sum += market->getPrice(i) * calculateExponentialWeight(i);
        }
        return double(sum / weight_sum);
    }
    for (int i = 0; i < window; i++)
    {
        weight_sum += calculateExponentialWeight(i);
        sum += market->getPrice(index - window + 1 + i) * calculateExponentialWeight(i);
    }
    return double(sum / weight_sum);
}

WeightedTrendFollowingStrategy **WeightedTrendFollowingStrategy::generateStrategySet(const string &baseName, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow)
{
    int num_ShortWindow = (maxShortWindow - minShortWindow) / stepShortWindow + 1;
    int num_LongWindow = (maxLongWindow - minLongWindow) / stepLongWindow + 1;
    int num = num_LongWindow * num_ShortWindow;
    WeightedTrendFollowingStrategy **StrategySet = new WeightedTrendFollowingStrategy *[num];
    int count = 0;
    for (int curr_shortwindow = minShortWindow; curr_shortwindow <= maxShortWindow; curr_shortwindow += stepShortWindow)
    {
        for (int curr_longwindow = minLongWindow; curr_longwindow <= maxLongWindow; curr_longwindow += stepLongWindow)
        {
            StrategySet[count++] = new WeightedTrendFollowingStrategy(baseName + "_" + to_string(curr_shortwindow) + "_" + to_string(curr_longwindow), curr_shortwindow, curr_longwindow);
        }
    }
    return StrategySet;
}
