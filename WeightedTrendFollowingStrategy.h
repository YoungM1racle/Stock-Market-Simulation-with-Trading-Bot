#ifndef WEIGHTED_TREND_FOLLOWING_STRATEGY_H
#define WEIGHTED_TREND_FOLLOWING_STRATEGY_H

#include "TrendFollowingStrategy.h"

class WeightedTrendFollowingStrategy : public TrendFollowingStrategy
{
private:
    double calculateExponentialWeight(int index) const;

public:
    WeightedTrendFollowingStrategy();
    WeightedTrendFollowingStrategy(const string &name, int shortWindow, int longWindow);
    double calculateMovingAverage(Market *market, int index, int window) const override;
    static WeightedTrendFollowingStrategy **generateStrategySet(const string &name, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow);
};

#endif // WEIGHTED_TREND_FOLLOWING_STRATEGY_H
