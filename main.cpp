#include <iostream>
#include <cassert>
#include <cmath>

#ifdef _WIN32
#include <windows.h>
#endif

#include "Strategy.h"
#include "Market.h"
#include "TradingBot.h"
#include "MeanReversionStrategy.h"
#include "TrendFollowingStrategy.h"
#include "WeightedTrendFollowingStrategy.h"
#include "Utils.h"

using namespace std;

// To avoid encoding issues on Windows, while Mac/Linux is fine with this issue
void setupWindows()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

bool compareDouble(double a, double b)
{
    double epsilon = 1e-6;
    return fabs(a - b) < epsilon;
}

int main()
{

    setupWindows();

    cout << "Please input test case number: ";
    int testid;
    cin >> testid;
    cout << endl;

    switch (testid)
    {
    case 0:
    {
        // Case 0: Generate basic testing cases. (Don't run it.)
        // Generate market according to bullish/bearish and low/high volatility

        // Bullish, Low Volatility
        Market *market1 = new Market(100.0, 0.15, 1.0, TRADING_DAYS_PER_YEAR, 999);
        market1->simulate();
        market1->writeToFile("bullish_low_vol.txt");

        // Bullish, High Volatility
        Market *market2 = new Market(100.0, 0.40, 1.0, TRADING_DAYS_PER_YEAR, 999);
        market2->simulate();
        market2->writeToFile("bullish_high_vol.txt");

        // Bearish, Low Volatility
        Market *market3 = new Market(100.0, 0.15, -0.8, TRADING_DAYS_PER_YEAR, 999);
        market3->simulate();
        market3->writeToFile("bearish_low_vol.txt");

        // Bearish, High Volatility
        Market *market4 = new Market(100.0, 0.40, -0.8, TRADING_DAYS_PER_YEAR, 999);
        market4->simulate();
        market4->writeToFile("bearish_high_vol.txt");

        break;
    }
    case 1:
    {
        // Test Case 1 - Testing functionality of Market class
        Market *market = new Market(100.0, 0.2, 1, TRADING_DAYS_PER_YEAR, 999);
        market->simulate();

        // print out all prices
        double **prices = market->getPrices();
        for (int i = 0; i < TRADING_DAYS_PER_YEAR; i++)
        {
            cout << "Day " << i << ": " << *prices[i] << endl;
        }

        delete market;
        cout << "Test case 1 done" << endl;

        break;
    }
    case 2:
    {
        // Test Case 2 - Testing Market Simulate Function
        Market *simulatedMarket = new Market(100.0, 0.15, 1.0, TRADING_DAYS_PER_YEAR, 999);
        simulatedMarket->simulate();

        // testing loading from file
        Market *loadedMarket = new Market(0, 0, 0, TRADING_DAYS_PER_YEAR, 999);
        loadedMarket->loadFromFile("bullish_low_vol.txt");

        cout << "Simulated market last price: " << simulatedMarket->getLastPrice() << endl;
        cout << "Loaded market last price: " << loadedMarket->getLastPrice() << endl;
        cout << "Simulated market volatility: " << simulatedMarket->getVolatility() << endl;
        cout << "Loaded market volatility: " << loadedMarket->getVolatility() << endl;
        cout << "Simulated market expected yearly return: " << simulatedMarket->getExpectedYearlyReturn() << endl;
        cout << "Loaded market expected yearly return: " << loadedMarket->getExpectedYearlyReturn() << endl;

        delete simulatedMarket;
        delete loadedMarket;
        cout << "Test case 2 done" << endl;
        break;
    }
    case 3:
    {
        // Test case 3 - Testing Strategy class & TradingBot class

        Market *market = new Market(0, 0, 0, TRADING_DAYS_PER_YEAR, 999);
        market->loadFromFile("bullish_low_vol.txt");
        TradingBot *tradingBot = new TradingBot(market);

        // Add strategies
        tradingBot->addStrategy(new MeanReversionStrategy("Mean Reversion 1", 10, 5));  // Updated threshold
        tradingBot->addStrategy(new MeanReversionStrategy("Mean Reversion 2", 15, 10)); // Updated threshold
        tradingBot->addStrategy(new MeanReversionStrategy("Mean Reversion 3", 5, 50));  // Updated threshold

        tradingBot->addStrategy(new TrendFollowingStrategy("Trend Following 1", 10, 15));
        tradingBot->addStrategy(new TrendFollowingStrategy("Trend Following 2", 20, 25));
        tradingBot->addStrategy(new TrendFollowingStrategy("Trend Following 3", 15, 25));

        // Add delta hedging strategies
        tradingBot->addStrategy(new WeightedTrendFollowingStrategy("Weighted Trend Following 1", 10, 15));
        tradingBot->addStrategy(new WeightedTrendFollowingStrategy("Weighted Trend Following 2", 20, 25));
        tradingBot->addStrategy(new WeightedTrendFollowingStrategy("Weighted Trend Following 3", 15, 25));

        // Run simulation
        SimulationResult result = tradingBot->runSimulation();

        cout << "Best strategy: " << result.bestStrategy->getName() << endl;
        cout << "Best return: " << result.totalReturn << endl;

        delete market; // This will also delete the option since market owns it
        delete tradingBot;

        cout << "Test case 3 done" << endl;
        break;
    }
    case 4:
    {
        // Test case 4 - Testing looping through all strategies' parameters
        Market *market = new Market(0, 0, 0, TRADING_DAYS_PER_YEAR, 999);
        market->loadFromFile("bullish_low_vol.txt");
        TradingBot *tradingBot = new TradingBot(market);

        // Generate and add strategies using separate functions
        WeightedTrendFollowingStrategy **weightedStrategies = WeightedTrendFollowingStrategy::generateStrategySet("WeightedTrend", 5, 15, 5, 20, 50, 10);
        for (int i = 0; i < 12; ++i)
        {
            tradingBot->addStrategy(weightedStrategies[i]);
        }
        delete[] weightedStrategies;

        TrendFollowingStrategy **trendStrategies = TrendFollowingStrategy::generateStrategySet("Trend", 5, 15, 5, 20, 100, 10);
        for (int i = 0; i < 27; ++i)
        {
            tradingBot->addStrategy(trendStrategies[i]);
        }
        delete[] trendStrategies;

        MeanReversionStrategy **meanReversionStrategies = MeanReversionStrategy::generateStrategySet("MeanReversion", 5, 15, 5, 1, 5, 1);
        for (int i = 0; i < 15; ++i)
        {
            tradingBot->addStrategy(meanReversionStrategies[i]);
        }
        delete[] meanReversionStrategies;

        // Run simulation
        SimulationResult result = tradingBot->runSimulation();

        cout << "Best strategy: " << result.bestStrategy->getName() << endl;
        cout << "Best return: " << result.totalReturn << endl;

        delete market;
        delete tradingBot;

        cout << "Test case 4 done" << endl;
        break;
    }
    case 5:
    {
        // Test case 5 - Testing looping through all strategies' parameters in a bearish market
        Market *market = new Market(0, 0, 0, TRADING_DAYS_PER_YEAR, 999);
        market->loadFromFile("bearish_low_vol.txt");
        TradingBot *tradingBot = new TradingBot(market);

        // Generate and add strategies using separate functions
        WeightedTrendFollowingStrategy **weightedStrategies = WeightedTrendFollowingStrategy::generateStrategySet("WeightedTrend", 5, 15, 5, 20, 50, 10);
        for (int i = 0; i < 12; ++i)
        {
            tradingBot->addStrategy(weightedStrategies[i]);
        }
        delete[] weightedStrategies;

        TrendFollowingStrategy **trendStrategies = TrendFollowingStrategy::generateStrategySet("Trend", 5, 15, 5, 20, 100, 10);
        for (int i = 0; i < 27; ++i)
        {
            tradingBot->addStrategy(trendStrategies[i]);
        }
        delete[] trendStrategies;

        MeanReversionStrategy **meanReversionStrategies = MeanReversionStrategy::generateStrategySet("MeanReversion", 5, 15, 5, 1, 5, 1);
        for (int i = 0; i < 15; ++i)
        {
            tradingBot->addStrategy(meanReversionStrategies[i]);
        }
        delete[] meanReversionStrategies;

        // Run simulation
        SimulationResult result = tradingBot->runSimulation();

        cout << "Best strategy: " << result.bestStrategy->getName() << endl;
        cout << "Best return: " << result.totalReturn << endl;

        delete market;
        delete tradingBot;

        cout << "Test case 5 done" << endl;
        break;
    }
    default:
    {
        cout << "Invalid test number!" << endl;
        break;
    }
    }
    return 0;
}