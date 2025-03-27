#ifndef TREND_FOLLOWING_STRATEGY_H
#define TREND_FOLLOWING_STRATEGY_H

#include "Strategy.h"
#include "Market.h"

class TrendFollowingStrategy : public Strategy
{
private:
    int shortMovingAverageWindow;
    int longMovingAverageWindow;

public:
    TrendFollowingStrategy();
    TrendFollowingStrategy(const string &name, int shortWindow, int longWindow);
    Action decideAction(Market *market, int index, double currentHolding) const override;
    static TrendFollowingStrategy **generateStrategySet(const string &name, int minShortWindow, int maxShortWindow, int stepShortWindow, int minLongWindow, int maxLongWindow, int stepLongWindow);
};

#endif // TREND_FOLLOWING_STRATEGY_H