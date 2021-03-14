//
// Created by Alvaro Diaz on 13-03-21.
//

#ifndef FX_TRADING_GAME_ORDER_H
#define FX_TRADING_GAME_ORDER_H

#include <string>

#include "PriceSource.h"

enum OrderType
{
    buy,
    sell
};

class Order
{
public:
    Order(std::string id, int amount, Price threshold);
    [[nodiscard]] bool isExecutable(Price price) const;
    virtual bool operator<(const Order& other)=0;
    [[nodiscard]] std::string playerName() const;
    [[nodiscard]] std::string id() const;
    [[nodiscard]] double amount() const;
    [[nodiscard]] Price threshold() const;
    [[nodiscard]] OrderType orderType() const;

protected:
    OrderType _orderType{OrderType::buy};
    double _amount;
    Price _threshold;
    std::string _id;
};

class BuyOrder : public Order
{
public:
    BuyOrder(std::string id, int amount, Price threshold);
    bool operator<(const Order& other) override;
};


class SellOrder : public Order
{
public:
    SellOrder(std::string id, int amount, Price threshold);
    bool operator<(const Order& other) override;
};


#endif //FX_TRADING_GAME_ORDER_H
