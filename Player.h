//
// Created by Alvaro Diaz on 06-03-21.
//

#ifndef FX_TRADING_GAME_PLAYER_H
#define FX_TRADING_GAME_PLAYER_H

#include <unordered_map>
#include <sstream>
#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <mutex>

#include "Auxiliary.h"
#include "Order.h"


class Player
{
public:
    Player() = default;
    explicit Player(std::string name);
    [[nodiscard]] std::string getName() const;
    std::shared_ptr<Order> newOrder(OrderType orderType, Amount amount, Price price);
    [[nodiscard]] int getLastOrderNumber() const;
    [[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Order>>& getOrders() const;
    void updatePosition(const std::string& orderId, Price price);
    [[nodiscard]] std::stringstream stats(Quote quote) const;

private:
    std::string _name;
    std::unordered_map<std::string, std::shared_ptr<Order>> _orders;
    int _lastOrder;
    std::mutex _mtx;
    struct _trade
    {
        _trade(std::string id, int amount, Price price, Price mktPrice) : myId(std::move(id)), myAmount(amount),
        myPrice(price), myMktPrice(mktPrice)
        {};
        std::string myId;
        int myAmount;
        Price myPrice;
        Price myMktPrice;
    };
    std::vector<_trade> _trades;
};


#endif //FX_TRADING_GAME_PLAYER_H
