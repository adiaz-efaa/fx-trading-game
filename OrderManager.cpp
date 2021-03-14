//
// Created by Alvaro Diaz on 13-03-21.
//
#include <iostream>
#include <utility>

#include "OrderManager.h"


OrderManager::OrderManager(std::shared_ptr<PriceSource> priceSource,
                           std::unordered_map<std::string, Player>& players) :
                           _priceSource(std::move(priceSource)),
                           _players(players)
{

}


void OrderManager::receive(std::shared_ptr<Order> order)
{
    std::unique_lock<std::mutex> lk(_mutex);
    std::cout << "Receiving order: " << order->id() << "\n";

    // Wait until processOrders has finished
    std::cout << "Waiting for processOrders ... \n";
    _cond.wait(lk, [this] { return _finishedProcessing; });

    switch (order->orderType())
    {
        case OrderType::buy:
            _buyOrders.push_back(order);
            std::sort(_buyOrders.begin(), _buyOrders.end(), [](
                    const std::shared_ptr<Order>& lhs,
                    const std::shared_ptr<Order>& rhs) { return *lhs < *rhs; });
            break;

        case OrderType::sell:
            _sellOrders.push_back(order);
            std::sort(_sellOrders.begin(), _sellOrders.end(), [](
                    const std::shared_ptr<Order>& lhs,
                    const std::shared_ptr<Order>& rhs) { return *lhs < *rhs; });
            break;
    }
    std::cout << "Order: " << order->id() << " received.\n";
    lk.unlock();
    _cond.notify_one();
}


const std::vector<std::shared_ptr<Order>>& OrderManager::buyOrders()
{
    return _buyOrders;
}


const std::vector<std::shared_ptr<Order>>& OrderManager::sellOrders()
{
    return _sellOrders;
}

void OrderManager::_processOrders()
{
    std::unique_lock<std::mutex> lk(_mutex);
    if (!_buyOrders.empty())
    {
        for (auto rit = _buyOrders.rbegin(); rit != _buyOrders.rend(); ++rit )
        {
            auto quote = _priceSource->getLastQuote();
            if (rit->get()->isExecutable(quote.ask))
            {
                auto playerName = rit->get()->playerName();
               _players.at(playerName).updatePosition(rit->get()->id(), quote.ask);
               _buyOrders.pop_back();
            }
        }
    }
    if (!_sellOrders.empty())
    {
        for (auto rit = _sellOrders.rbegin(); rit != _sellOrders.rend(); ++rit )
        {
            auto quote = _priceSource->getLastQuote();
            if (rit->get()->isExecutable(quote.bid))
            {
                auto playerName = rit->get()->playerName();
                _players.at(playerName).updatePosition(rit->get()->id(), quote.bid);
                _sellOrders.pop_back();
            }
        }
    }

    // Notify
    _finishedProcessing = true;
    lk.unlock();
    _cond.notify_one();
}


void OrderManager::startProcessing()
{
    _stopProcessing = false;
    _processOrders();
    auto lastQuote = _priceSource->getLastQuote();
    auto lastNumBuyOrders = _buyOrders.size();
    auto lastNumSellOrders = _sellOrders.size();
    while (!_stopProcessing)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto newQuote = _priceSource->getLastQuote();
        auto newNumBuyOrders = _buyOrders.size();
        auto newNumSellOrders = _sellOrders.size();
        if (newQuote != lastQuote || newNumBuyOrders > lastNumBuyOrders || newNumSellOrders > lastNumSellOrders)
        {
            _processOrders();
        }
        lastQuote = newQuote;
        lastNumBuyOrders = _buyOrders.size();
        lastNumSellOrders = _sellOrders.size();
    }
}


void OrderManager::stopProcessing()
{
    _stopProcessing = true;
}


OrderManager::~OrderManager()
{
    std::cout << "Order manager destructed.\n";
}

