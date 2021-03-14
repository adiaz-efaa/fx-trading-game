//
// Created by Alvaro Diaz on 13-03-21.
//

#include "Order.h"

#include <utility>

Order::Order(std::string id, int amount, Price threshold)
        : _id(std::move(id)), _amount(amount), _threshold(threshold)
{
}


bool Order::isExecutable(Price price) const
{
    switch (_orderType)
    {
        case OrderType::buy:
            return price <= _threshold;

        case OrderType::sell:
            return price >= _threshold;
    }
}


double Order::amount() const
{
    return _amount;
}


Price Order::threshold() const
{
    return _threshold;
}


std::string Order::id() const
{
    return _id;
}


OrderType Order::orderType() const
{
    return _orderType;
}


std::string Order::playerName() const
{
    auto pos = _id.find('-');
    return _id.substr(0, pos);
}


BuyOrder::BuyOrder(std::string id, int amount, Price threshold) : Order(std::move(id), amount, threshold)
{
    _orderType = OrderType::buy;
}


bool BuyOrder::operator<(const Order& other)
{
    return _threshold < other.threshold();
}


SellOrder::SellOrder(std::string id, int amount, Price threshold) : Order(std::move(id), amount, threshold)
{
    _orderType = OrderType::sell;
}


bool SellOrder::operator<(const Order& other)
{
    return _threshold > other.threshold();
}

