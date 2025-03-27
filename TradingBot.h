#ifndef TRADING_BOT_H
#define TRADING_BOT_H

#include <vector>
#include "Strategy.h"
#include "Market.h"
#include "TrendFollowingStrategy.h"
#include "WeightedTrendFollowingStrategy.h"
#include "MeanReversionStrategy.h"

struct SimulationResult
{
    Strategy *bestStrategy;
    double totalReturn;

    SimulationResult() : bestStrategy(nullptr),
                         totalReturn(-std::numeric_limits<double>::max()) {}
};

class TradingBot
{
private:
    Market *market;
    Strategy **availableStrategies;
    int strategyCount;
    int strategyCapacity;

public:
    TradingBot(Market *market, int initialCapacity = 10);
    ~TradingBot();

    void addStrategy(Strategy *strategy);
    SimulationResult runSimulation();

    // Prevent copying
    TradingBot(const TradingBot &) = delete;
    TradingBot &operator=(const TradingBot &) = delete;
};

#endif