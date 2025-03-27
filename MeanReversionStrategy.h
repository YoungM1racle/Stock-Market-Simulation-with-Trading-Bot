#ifndef MEAN_REVERSION_STRATEGY_H
#define MEAN_REVERSION_STRATEGY_H

#include "Strategy.h"
#include "Market.h"

class MeanReversionStrategy : public Strategy
{
private:
    int window;
    int threshold;

public:
    MeanReversionStrategy();
    MeanReversionStrategy(const string &name, int window, int threshold);
    Action decideAction(Market *market, int index, double currentHolding) const override;
    static MeanReversionStrategy **generateStrategySet(const string &baseName, int minWindow, int maxWindow, int windowStep, int minThreshold, int maxThreshold, int thresholdStep);
};

#endif