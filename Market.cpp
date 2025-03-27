#include "Market.h"
#include "Utils.h"

Market::Market(double initialPrice, double volatility, double expectedYearlyReturn, int numTradingDays, int seed) : initialPrice(initialPrice), volatility(volatility), expectedYearlyReturn(expectedYearlyReturn), numTradingDays(numTradingDays), seed(seed)
{
    prices = new double *[numTradingDays];
    for (int i = 0; i < numTradingDays; i++)
        prices[i] = new double(0.0);
}

Market::Market(const string &filename)
{
    loadFromFile(filename);
}

Market::~Market()
{
    if (prices)
    {
        for (int i = 0; i < numTradingDays; i++)
            delete prices[i];
        delete[] prices;
    }
}

// ===== Don't modify below this line =====
// Helper function to generate a random number from a normal distribution
double Market::generateZ(int seed)
{
    static std::mt19937 gen(seed == -1 ? std::random_device{}() : seed);
    static std::normal_distribution<> d(0, 1);
    return d(gen);
}
// ===== Don't modify above this line =====

void Market::simulate()
{
    double delta_t = double(1.0 / TRADING_DAYS_PER_YEAR);

    *prices[0] = initialPrice;
    for (int i = 0; i < numTradingDays - 1; i++)
    {
        double Z = generateZ(seed);
        *prices[i + 1] = roundToDecimals(*prices[i] * exp((expectedYearlyReturn - 0.5 * volatility * volatility) * delta_t + volatility * sqrt(delta_t) * Z), 3);
    }
}

double Market::getVolatility() const
{
    return volatility;
}

double Market::getExpectedYearlyReturn() const
{
    return expectedYearlyReturn;
}

double **Market::getPrices() const
{
    return prices;
}

double Market::getPrice(int index) const
{
    return *prices[index];
}

double Market::getLastPrice() const
{
    int index;
    for (index = 0; index < numTradingDays; index++)
    {
        if (*prices[index] == 0.0)
            break;
    }
    if (index == 0)
        return initialPrice;
    return *prices[index - 1];
}

int Market::getNumTradingDays() const
{
    return numTradingDays;
}

// ===== Don't modify below this line =====
void Market::createDirectory(const string &folder)
{
#ifdef _WIN32
    _mkdir(folder.c_str());
#else
    mkdir(folder.c_str(), 0777);
#endif
}

void Market::writeToFile(const string &filename)
{
    string folder = "data";
    string filePath = folder + "/" + filename;

    createDirectory(folder);

    ofstream outFile(filePath);
    if (!outFile)
    {
        cerr << "Error opening file for writing: " << filePath << endl;
        return;
    }

    outFile << initialPrice << " " << volatility << " "
            << expectedYearlyReturn << " " << numTradingDays << " " << seed << endl;

    for (int i = 0; i < numTradingDays; ++i)
    {
        outFile << *prices[i] << endl;
    }

    outFile.close();
    cout << "Market parameters and prices written to file: " << filePath << endl;
}

void Market::loadFromFile(const string &filename)
{
    string filePath = "data/" + filename;
    ifstream inFile(filePath);
    if (!inFile)
    {
        cerr << "Error opening file for reading: " << filePath << endl;
        return;
    }

    inFile >> initialPrice >> volatility >> expectedYearlyReturn >> numTradingDays >> seed;

    // Clear existing prices
    if (prices)
    {
        for (int i = 0; i < numTradingDays; i++)
        {
            delete prices[i];
        }
        delete[] prices;
    }

    // Count number of prices first
    ifstream countFile(filePath);
    string line;
    int count = 0;
    double dummy;
    countFile >> dummy >> dummy >> dummy >> dummy >> dummy; // Skip parameters
    while (countFile >> dummy)
        count++;
    countFile.close();

    // Allocate new array
    prices = new double *[count];
    for (int i = 0; i < count; i++)
    {
        prices[i] = new double(0.0);
    }

    // Read prices
    int pricesSize = 0;
    double price;
    while (inFile >> price)
    {
        *prices[pricesSize++] = price;
    }

    inFile.close();
    cout << "Loaded parameters from file: " << filePath << endl;
    cout << "Initial Price: " << initialPrice << ", Volatility: " << volatility
         << ", Expected Yearly Return: " << expectedYearlyReturn
         << ", Num of Trading Days: " << numTradingDays << ", Seed: " << seed << endl;
    cout << "Loaded " << pricesSize << " price entries." << endl;
}
// ===== Don't modify above this line =====

// TODO: Implement the member functions of the Market class
