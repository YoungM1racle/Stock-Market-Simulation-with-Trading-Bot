#ifndef MARKET_H
#define MARKET_H

#include <iostream>
#include <cmath>
#include <random>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#include <direct.h> // For mkdir on Windows
#else
#include <sys/stat.h> // For mkdir on Unix/Linux/Mac
#endif

using namespace std;

class Market
{

private:
    double initialPrice;
    double volatility;
    double expectedYearlyReturn;
    int numTradingDays;
    double **prices = nullptr;
    // int pricesSize = 0;
    int seed = -1;

    double generateZ(int seed);
    void createDirectory(const string &folder);

public:
    Market(double initialPrice, double volatility, double expectedYearlyReturn, int numTradingDays, int seed = -1);
    Market(const string &filename);
    ~Market();

    void simulate();
    void writeToFile(const string &filename);
    void loadFromFile(const string &filename);
    double getVolatility() const;
    double getExpectedYearlyReturn() const;
    double **getPrices() const;
    double getPrice(int index) const;
    double getLastPrice() const;
    int getNumTradingDays() const;
};

#endif