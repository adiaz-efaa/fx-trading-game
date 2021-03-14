//
// Created by Alvaro Diaz on 06-03-21.
//

#ifndef FX_TRADING_GAME_AUXILIARY_H
#define FX_TRADING_GAME_AUXILIARY_H

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <mutex>

#include "Order.h"

namespace aux
{
    std::vector<std::string> splitCommand(std::string s);

    enum CommandEnum
    {
        getQuote,
        newPlayer,
        newOrder,
        lastOrder,
        ordersFrom,
        allBuyOrders,
        allSellOrders,
        position,
        message,
        quit
    };

    std::string timePointAsString(const std::chrono::time_point<std::chrono::system_clock>& tp);

    void displayOrders(const std::vector<std::shared_ptr<Order>>& orders);

    void displayOrders(const std::unordered_map<std::string, std::shared_ptr<Order>>& orders);
}

#endif //FX_TRADING_GAME_AUXILIARY_H
