//
// Created by Alvaro Diaz on 06-03-21.
//

#include <unordered_map>
#include <iostream>
#include <sstream>
#include <memory>
#include <thread>

#include "Auxiliary.h"
#include "OrderManager.h"
#include "PriceSource.h"
#include "Player.h"

std::string getCommand()
{
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int main()
{
    // Create prompt
    std::string prompt = "fxgame> ";

    // Start price simulation
    Simulator simulator{.01, .005, .12, 10};
    auto priceSource = std::make_shared<PriceSource>(std::move(simulator));
    std::thread priceFeed([&](){
        priceSource->startPriceSimulation();
    });
    priceFeed.detach();

    // Create unordered_map of players
    std::unordered_map<std::string, Player> players;

    // Create and start order manager
    auto orderManager = std::make_shared<OrderManager>(priceSource, players);
    std::thread orderLoop([&] {
        orderManager->startProcessing();
    });
    orderLoop.detach();

    std::cout << "Order loop id: " << orderLoop.get_id() << "\n";
    std::cout << "Price source id: " << priceFeed.get_id() << "\n";

    // Create and unordered_map of commands
    std::unordered_map<std::string, aux::CommandEnum> commandNumbers;
    commandNumbers.insert(std::make_pair("new_player", aux::CommandEnum::newPlayer));
    commandNumbers.insert(std::make_pair("get_quote", aux::CommandEnum::getQuote));
    commandNumbers.insert(std::make_pair("q", aux::CommandEnum::getQuote));
    commandNumbers.insert(std::make_pair("new_order", aux::CommandEnum::newOrder));
    commandNumbers.insert(std::make_pair("last_order", aux::CommandEnum::lastOrder));
    commandNumbers.insert(std::make_pair("orders_from", aux::CommandEnum::ordersFrom));
    commandNumbers.insert(std::make_pair("all_buy_orders", aux::CommandEnum::allBuyOrders));
    commandNumbers.insert(std::make_pair("all_sell_orders", aux::CommandEnum::allSellOrders));
    commandNumbers.insert(std::make_pair("position", aux::CommandEnum::position));
    commandNumbers.insert(std::make_pair("msg", aux::CommandEnum::message));
    commandNumbers.insert(std::make_pair("quit", aux::CommandEnum::quit));

    std::string cmd;
    bool stop = false;
    std::mutex _outMutex;
    while (!stop)
    {
        std::lock_guard<std::mutex> lock(_outMutex);
        std::cout << prompt;
        _outMutex.unlock();
        std::future<std::string> futureString = std::async(std::launch::async, getCommand);
        futureString.wait();
        cmd = futureString.get();
        auto args = aux::splitCommand(cmd);
        auto iter = commandNumbers.find(args[0] );
        if (iter != commandNumbers.end())
        {
            switch (commandNumbers.at(args[0]))
            {
                case aux::CommandEnum::newPlayer:
                    if (!args[1].empty())
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        players.emplace(args[1], args[1]);
                        std::cout << "Player: \"" << args[1] << "\" successfully added.\n";
                    }
                    else
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        std::cout << "Player must have a name.\n";
                    }
                    break;

                case aux::CommandEnum::getQuote:
                {
                    auto q = priceSource->getLastQuote();
                    std::lock_guard<std::mutex> lockGuard(_outMutex);
                    std::cout << "Bid: " << q.bid << ", " << "Ask: " << q.ask << "\n";
                    break;
                }

                case aux::CommandEnum::newOrder:
                {
                    auto orderType1 = (args[2] == "b") ? OrderType::buy : OrderType::sell;
                    try
                    {
                        auto newOrder = (players.at(args[1])).newOrder(
                                orderType1, stod(args[3]), stod(args[4]));
                        orderManager->receive(newOrder);
                    }
                    catch (...)
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        std::cout << "Bad command. Correct usage is:\n";
                        std::cout << "new_order <player_name> <b or s> <amount> <price>" << std::endl;
                    }
                    break;
                }

                case aux::CommandEnum::lastOrder:
                {
                    try
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        std::cout << "Last order number: " << (players.at(args[1])).getLastOrderNumber() << "\n";
                    }
                    catch (...)
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        std::cout << "Bad command. Correct usage is:\n";
                        std::cout << "last_order <player_name>" << std::endl;
                    }
                    break;
                }

                case aux::CommandEnum::ordersFrom:
                {
                    try
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        auto orders = players.at(args[1]).getOrders();
                        aux::displayOrders(orders);
                    }
                    catch (...)
                    {
                        std::lock_guard<std::mutex> lockGuard(_outMutex);
                        std::cout << "Bad command. Correct usage is:\n";
                        std::cout << "orders_from <player_name>" << std::endl;
                    }
                    break;
                }

                case aux::CommandEnum::allBuyOrders:
                {
                    std::lock_guard<std::mutex> lockGuard(_outMutex);
                    auto orders = orderManager->buyOrders();
                    aux::displayOrders(orders);
                    break;
                }

                case aux::CommandEnum::allSellOrders:
                {
                    std::lock_guard<std::mutex> lockGuard(_outMutex);
                    auto orders = orderManager->sellOrders();
                    aux::displayOrders(orders);
                    break;
                }

                case aux::CommandEnum::position:
                    try
                    {
                        std::cout << players.at(args[1]).stats(priceSource->getLastQuote()).str();
                    }
                    catch (...)
                    {
                        std::cout << "Bad command. Correct usage is:\n";
                        std::cout << "position <player_name>" << std::endl;
                    }

                    break;

                case aux::CommandEnum::message:
                    {
                        std::stringstream out;
                        for (auto it = args.begin() + 1; it != args.end();  ++it )
                        {
                            out << *it;
                        }
                        auto outString = out.str();
                        if (outString.size() <= 1)
                        {
                            // hgggg
                            out << "No messages";
                            std::cout << out.str() << std::endl;
                            break;
                        }
                        std::cout << outString << std::endl;
                        break;

                    }

                case aux::CommandEnum::quit:
                {
                    stop = true;
                }
            }
        }
        else
        {
            std::lock_guard<std::mutex> lockGuard(_outMutex);
            std::cout << "Unknown command " << args[0] << "\n";
        }
    }
    return 0;
}


