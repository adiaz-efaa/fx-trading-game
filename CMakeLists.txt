cmake_minimum_required(VERSION 3.17)
project(fx_trading_game)

set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads)

add_executable(fx_trading_game
        fxgame.cpp
        Player.cpp
        Auxiliary.cpp
        OrderManager.cpp
        PriceSource.cpp
        Order.cpp
        )

target_link_libraries(fx_trading_game Threads::Threads)
