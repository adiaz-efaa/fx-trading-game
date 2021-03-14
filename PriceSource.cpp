//
// Created by Alvaro Diaz on 13-03-21.
//

#include <iostream>
#include <thread>

#include "PriceSource.h"

Simulator::Simulator(double rd, double rf, double vol, int num_seconds) : _rd(rd), _rf(rf), _vol(vol),
_num_seconds(num_seconds)
{
    auto t = static_cast<uint32_t>( time(nullptr) );
    _gen.seed(t);

    _dt = num_seconds / _secondsInYear;
    _drift = exp((_rd - _rf - .5 * _vol * _vol) * _dt);
    _diffusion = _vol * sqrt(_dt);
}


double Simulator::nextPrice(Price price)
{
    return (int)(price * _drift * exp(_diffusion * _d(_gen)) * 100.0) / 100.0;
}


int Simulator::num_seconds()
{
    return _num_seconds;
}


PriceSource::PriceSource(std::filesystem::path  pathToFiles, Simulator&& simulator) :
        _pathToFiles(std::move(pathToFiles)), _simulator(std::move(simulator))
{
    _lastQuote.bid = 699.95;
    _lastQuote.ask = 700.05;
}


Quote PriceSource::_simulatePrice()
{
    auto mid = (_lastQuote.bid + _lastQuote.ask) / 2.0;
    mid = _simulator.nextPrice(mid);
    _lastQuote.bid = mid - .05;
    _lastQuote.ask = mid + .05;
    return _lastQuote;
}


void PriceSource::_storePrice(Quote quote)
{
    _lastQuote = quote;
    // std::cout << "Bid: " << _lastQuote.bid << ", Ask: " << _lastQuote.ask << " has been stored.\n";
}


void PriceSource::startPriceSimulation()
{
    _stopPriceSimulation = false;
    while (!_stopPriceSimulation)
    {
        std::this_thread::sleep_for(std::chrono::seconds(_simulator.num_seconds()));
        _storePrice(_simulatePrice());
    }
}


PriceSource::~PriceSource()
{
    std::cout << "Price source destructed.\n";
}


void PriceSource::stopPriceSimulation()
{
    _stopPriceSimulation = true;
}


Quote PriceSource::getLastQuote()
{
    return _lastQuote;
}
