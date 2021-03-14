//
// Created by Alvaro Diaz on 06-03-21.
//

#include "Player.h"

#include <iostream>
#include <utility>
#include <string>

Player::Player(std::string name) : _name(std::move(name)), _lastOrder(0)
{
}


std::shared_ptr<Order> Player::newOrder(OrderType orderType, Amount amount, Price price)
{
    std::lock_guard<std::mutex> lock(_mtx);
    std::shared_ptr<Order> order;
    if (orderType == OrderType::buy)
        order = std::make_shared<BuyOrder>(
                BuyOrder(_name + "-" + std::to_string(_lastOrder), amount, price));
    if (orderType == OrderType::sell)
        order = std::make_shared<SellOrder>(
                SellOrder(_name + "-" + std::to_string(_lastOrder), amount, price));
    ++_lastOrder;
    _orders.insert({order->id(), order});
    std::cout << "Order: " << order->id() << " added to player.\n";
    return order;
}


int Player::getLastOrderNumber() const
{
    return _lastOrder;
}


std::string Player::getName() const
{
    return _name;
}


const std::unordered_map<std::string, std::shared_ptr<Order>>& Player::getOrders() const
{
    return _orders;
}


void Player::updatePosition(const std::string& orderId, Price price)
{
    std::lock_guard<std::mutex> lockGuard(_mtx);
    auto fulfilledOrder= _orders.find(orderId);
    switch (fulfilledOrder->second->orderType())
    {
        case OrderType::buy:
        {
            auto s = fulfilledOrder->second;
            _trades.emplace_back(s->id(), s->amount(), s->threshold(), price);
            break;
        }

        case OrderType::sell:
        {
            auto s = fulfilledOrder->second;
            _trades.emplace_back(s->id(), -(s->amount()), s->threshold(), price);
            break;
        }
    }
    std::cout << "Player: " << _name << " update position with order: " << orderId;
}


std::stringstream Player::stats(Quote quote) const
{
    std::stringstream result;
    double position = 0.0;
    double pnl = 0.0;
    for (auto trade : _trades)
    {
        position += trade.myAmount;
        auto price = (position < 0) ? quote.ask : quote.bid;
        pnl += trade.myAmount * (price - trade.myPrice) ;
    }
    result << "Player: " << _name << "\n" << "Position: " << position << "\n" << "PnL: " << pnl << "\n";
    return result;

}
