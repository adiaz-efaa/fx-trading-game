# FX Trading Game

Text based game that simulates the mechanics of FX trading.

- There is a task running in the background that generates bod-offer prices for FX pair USDCLP (with almost no additional configuration, any FX pair could be simulated).
- There is an additional task in the background that manages buy and sell orders from the different players.
- Any player can issue a buy or sell order for a certain amount of USD at a certain price. For example, player A wants to BUY 1 MM USD at a price of 700.00 CLP/USD.
- In the previous example, if the ask price reaches a level greater than or equal to 700.00 the order will be fulfilled and player A's position will reflect the new trade.
- At any time time a player can check her/his position. The total PnL will also be displayed. This PnL can be thought of the player's score. The higher the PnL, the better the score.

## Installing

**PLEASE NOTE:** fx-trading-game needs Ubuntu 18.04 to compile and run properly. The program has also been tested on Mac OS 11.2.3.

Clone the repo: `git clone https://github.com/adiaz-efaa/fx-trading-game.git`

Once cloned, cd into the directory: `cd fx-trading-game`

Create a build directory: `mkdir build`

Cd into the build directory: `cd build`

Run CMAKE: `cmake ..`

RUN MAKE to compile: `make`

## Running

Execute: `./fx-trading-game`

## How to Play

When the program is run, you will see the following interactive session:

`fxgame> `

The price simulator and the order manager will immediately start running.

- To add a new player: `fxgame> new_player udacity`, the name of the new player will be *udacity*.
- To check the last price run `fxgame> q`. Bid and ask prices will be displayed. Remember that in order to buy you have to ckeck the *ask* price and in order to sell the *bid* price is the appropriate one.
- To issue a new buy order run `fxgame> new_order udacity b 1000 700`, where: `udacity` is the player's name, "b" stand for BUY, 1000 is the amount of USD the player wishes to buy and 700 is the price at which the player wishes to biy the specified amount fo USD. If the player wants to sell, he/she should use "s" instead of "b".
- The order manager will work in the background to check if the order can be fulfilled. In order to check last price and status of orders one should run `fxgame> msg`.
- At any time a player can check her/his position by running `fxgame> position udacity` where `udacity` is the player's name.
- To quit the game just run `fxgame> quit`.

## All Commands

- `new_player`: instantiated a new player. Usage is `fxgame> new_player udacity`, where `udacity` is the name of the new player.
- `new_order`: issues a new buy or sell order. Usage is `fxgame> new_order udacity b 1000 700.00` for a buy order of 1000 USD at 700.00 clp/USD or  `fxgame> new_order udacity s 1000 700.00` for a sell order of 1000 USD at 700.00. Both orders are issued by player `udacity`.
- `q`: get the last quote (bid and ask prices) generated by the price simulator. 
- `get_quote`: more verbose version of `q`.
- `last_order`: usage is `fxgame> last_order udacity`. Displays the number of last order issued by player `udacity`.
- `orders_from`: should be used followed by a player's name. For example `fxgame> orders_from udacity`. All the orders issued by the player `udacity` will be displayed.
- `all_buy_orders`: all buy orders issued by all the players will be displayed.
- `all_sell_orders`: all sell orders issued by all the players will be displayed.
- `position`: usage is `fxgame> position udacity`. The position and PnL of player `udacity` will be displayed.

## Project Structure

- `fxgame.cpp`: entry point of the program
- `class Order`: the class is implemented is separate .h and .cpp files. This is an abstract class that represents an Order issued by a player. There are are 2 classes derived from Order, BuyOrder and SellOrder which represent a buy and a sell order respectively. Both classes override operator< which is purely virtual in their parent class.
- `class Player`: the class is implemented is separate .h and .cpp files. This class represents the game's players. It keeps track of the orders issued by the player and calculates it's position and PnL.
- `class PriceSource`: the class is implemented is separate .h and .cpp files. With the help of class `Simulator`, this class is in charge of simulating the market prices against which players can place buy or sell orders. Since it is closely related to `PriceSource` class `Simulator` is implemented in these same files. This class implements a very small version of the dynamics of a Black-Scholes-Merton model specialized for FX rates.
- `class OrderManager`: the class is implemented is separate .h and .cpp files. This class manages orders issued by the players. As soon as order can be fulfilled (depending on it's price and simulated price), the player that issued the order is notified.
- `Auxiliary.h` and `Auxiliary.cpp`: some helper functions and objects. They are all gathered under `namespace` aux.

## Loops, Functions, I/O

- 'fxgame.cpp' implements a `while loop` (line 65) the games' loop.
- In 'namespace aux', file `Auxiliary.h` lines 20, 39 and 41 3 hekper functions are declared (they are implemented) in `Auxiliary.cpp`.
- No input is read from files. The user interacts with the game through commands entered at the game's prompt. See for instance `fxgame.cpp` lines 77 and onwards.

## Object Oriented Programming

- The program is organized into classes. See **Project Structure**.
- All class members are explicitly declared private, public or protected. See for example class `Order` in file `Order.h` line 18.
- When appropriate class constructors utilize initialization lists. See for example `OrderManager.cpp` line 10 (constructor for class `OrderManeger`).
- Function `displayOrders` is overloaded. See `Auxiliary.h` lines 39 and 41.

## Memory Management

- References:
  - the parameter `std::unordered_map<std::string, Player>& players` is received by the constructor of `OrderManager`, see `OrderManager.cpp` line 11.
  - method `[[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<Order>>& getOrders() const;` of class `Player` returns a `const &`. See `Player.h` line 28.
- There are no unmanaged dynamic memory allocations (raw pointers).
- See use of `shared_ptr` in `fxgame.cpp` lines 31 and 41.
- See the use of `unique_ptr` in `PriceSource.h` line 52.

## Concurrency

- The project uses 3 threads, main thread, thread 2 where `PriceSource` runs and thread 3 where `OrderManager` runs. See `fxgame.cpp` lines 32 and 42.
- A future is used to wait for user input in main loop. See `fxgame.cpp` line 69.
- A mutex is used in main loop to protect `std::cout`. See `fxgame.cpp` lines 68 and 81 for example.
- Class `OrderManager` uses a condition variable to regulate access to orders in methods `receive` and `_process_orders`. See `OrderManager.cpp` line 26.
