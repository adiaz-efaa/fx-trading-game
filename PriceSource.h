//
// Created by Alvaro Diaz on 13-03-21.
//

#ifndef FX_TRADING_GAME_PRICESOURCE_H
#define FX_TRADING_GAME_PRICESOURCE_H

#include <random>

using Price = double;
using Amount = double;

struct Quote
{
    Price bid;
    Price ask;
    void moveBy(float amount)
    {
        bid += amount;
        ask += amount;
    }


    bool operator==(const Quote& rhs) const
    {
        return bid == rhs.bid &&
               ask == rhs.ask;
    }


    bool operator!=(const Quote& rhs) const
    {
        return !(rhs == *this);
    }
};

class Simulator
{
public:
    Simulator(double rd, double rf, double vol, int num_seconds);
    double nextPrice(Price price);
    int num_seconds();

private:
    double _rd;
    double _rf;
    double _vol;
    double _dt;
    double _drift;
    double _diffusion;
    int _num_seconds;
    std::mt19937 _gen;
    std::normal_distribution<> _d{0, 1};
    const double _secondsInYear{252 * 24 * 60 * 60}; // 252 is a proxy for the number of working days in a year.
};


class PriceSource
{
public:
    PriceSource(Simulator&& simulator);
    virtual ~PriceSource();
    void startPriceSimulation();
    Quote getLastQuote();
    void stopPriceSimulation();


private:
    Quote _simulatePrice();
    void _storePrice(Quote quote);
    Simulator _simulator;
    Quote _lastQuote{};
    bool _stopPriceSimulation{false};
};



#endif //FX_TRADING_GAME_PRICESOURCE_H
