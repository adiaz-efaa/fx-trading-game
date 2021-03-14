//
// Created by Alvaro Diaz on 13-03-21.
//

#ifndef FX_TRADING_GAME_ORDERMANAGER_H
#define FX_TRADING_GAME_ORDERMANAGER_H

#include <unordered_map>
#include <algorithm>
#include <future>
#include <map>

#include "PriceSource.h"
#include "Player.h"
#include "Order.h"


class OrderManager
{
public:
    OrderManager() = delete;
    explicit OrderManager(std::shared_ptr<PriceSource> priceSource,
                          std::unordered_map<std::string, Player>& players);

    virtual ~OrderManager();

    void receive(std::shared_ptr<Order> order);

    const std::vector<std::shared_ptr<Order>>& buyOrders();
    const std::vector<std::shared_ptr<Order>>& sellOrders();

    void startProcessing();
    void stopProcessing();

private:
    std::shared_ptr<PriceSource> _priceSource;
    std::vector<std::shared_ptr<Order>> _buyOrders;
    std::vector<std::shared_ptr<Order>> _sellOrders;
    std::unordered_map<std::string, Player>& _players;
    void _processOrders();
    std::mutex _mutex;
    std::condition_variable _cond;
    bool _finishedProcessing{false};
    bool _stopProcessing{false};
};



#endif //FX_TRADING_GAME_ORDERMANAGER_H
