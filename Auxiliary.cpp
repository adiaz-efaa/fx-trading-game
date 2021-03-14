//
// Created by Alvaro Diaz on 06-03-21.
//

#include "Auxiliary.h"

namespace aux
{
    std::vector<std::string> splitCommand(std::string s)
    {
        std::vector<std::string> result;
        std::istringstream iss(s);
        for (std::string s; iss >> s;)
            result.push_back(s);
        return result;
    }

    std::string timePointAsString(const std::chrono::time_point<std::chrono::system_clock>& tp)
    {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::string ts = std::ctime(&t);
        ts.resize(ts.size()-1);
        return ts;
    }


    void displayOrders(const std::vector<std::shared_ptr<Order>>& orders)
    {
        for (const auto& ord: orders)
        {
            std::cout << ord->id() << ", " << ord->amount() << ", " << ord->threshold() << "\n";
        }
    }


    void displayOrders(const std::unordered_map<std::string, std::shared_ptr<Order>>& orders)
    {
        for (const auto& p : orders)
        {
            std::cout << p.second->id() << ", " << p.second->amount() << ", " << p.second->threshold() << "\n";
        }
    }
}

