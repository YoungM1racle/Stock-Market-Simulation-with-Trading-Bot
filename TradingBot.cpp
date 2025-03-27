#include "TradingBot.h"
#include <limits>

TradingBot::TradingBot(Market *market, int initialCapacity) : market(market), strategyCapacity(initialCapacity), strategyCount(0)
{
    availableStrategies = new Strategy *[strategyCapacity];
    for (int i = 0; i < strategyCapacity; i++)
    {
        availableStrategies[i] = nullptr;
    }
}

TradingBot::~TradingBot()
{
    for (int i = 0; i < strategyCount; i++)
    {
        delete availableStrategies[i];
    }
    delete[] availableStrategies;
}

void TradingBot::addStrategy(Strategy *strategy)
{
    if (strategyCount == strategyCapacity)
    {
        strategyCapacity *= 2;
        Strategy **new_availableStrategies = new Strategy *[strategyCapacity];
        for (int i = 0; i < strategyCount; i++)
        {
            new_availableStrategies[i] = availableStrategies[i];
        }
        for (int i = strategyCount; i < strategyCapacity; i++)
            new_availableStrategies[i] = nullptr;
        delete[] availableStrategies;
        availableStrategies = new_availableStrategies;
        availableStrategies[strategyCount++] = strategy;
        return;
    }
    availableStrategies[strategyCount++] = strategy;
}

SimulationResult TradingBot::runSimulation()
{
    SimulationResult result;
    if (strategyCount == 0 || market == nullptr)
        return result;
    /*double simu_prices[101];
    for (int i = 100; i >= 0; i--)
    {
        simu_prices[i] = market->getPrice(i + market->getNumTradingDays() - 101);
    }*/
    for (int i = 0; i < strategyCount; i++)
    {
        Strategy *curr_strategy = availableStrategies[i];
        double cumulative_profit = 0.0;
        bool isHolding = 0;
        double buy_price = 0;
        for (int j = 0; j < 101; j++)
        {
            Action act = curr_strategy->decideAction(market, j + market->getNumTradingDays() - 101, isHolding);
            switch (act)
            {
            case BUY:
            {
                if (!isHolding)
                {
                    isHolding = true;
                    buy_price = market->getPrice(j + market->getNumTradingDays() - 101);
                }
                break;
            }
            case SELL:
            {
                if (isHolding)
                {
                    isHolding = false;
                    cumulative_profit += market->getPrice(j + market->getNumTradingDays() - 101) - buy_price;
                }
                break;
            }
            case HOLD:
            {
                break;
            }
            }
        }
        if (isHolding)
        {
            cumulative_profit += market->getLastPrice() - buy_price;
        }
        if (cumulative_profit > result.totalReturn)
        {
            result.totalReturn = cumulative_profit;
            result.bestStrategy = curr_strategy;
        }
    }
    return result;
}