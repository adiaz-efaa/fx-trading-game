# FX Trading Game

Text based game that simulates the mechanics of FX trading.

- There is a task running in the background that generates bod-offer prices for FX pair USDCLP (with almost no additional configuration, any FX pair could be simulated).
- There is an additional task in the background that maneges buy and sell orders from the different players.
- Any player can issue a buy or sell order for a certain amount of USD at a certain price. For example, player A wants to BUY 1 MM USD at a price of 700.0 CLP/USD.
- In the previous example, if the ask price reaches a level greater than or equal to 700.00 the order will be fulfilled and player A's position will reflect the new trade.
- At any time time a player can check her/his position. The total PnL will also be displayed. This PnL can be thought of the player's score. The higher the PnL, the better the score.

## Installing

Clone the repo: `git clone https://github.com/adiaz-efaa/fx-trading-game.git`

Once cloned cd into the directory: `cd fx-trading-game`

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
- To issue a new buy order run `fxgame> new_order udacity b 1000 700`, where: `udacity` is the player's name, "b" stand for BUY, 1000 is the amount of USD the player wishes to buy and 700 is the price at which the player wishes to biy the specified amount fo USD. If the player wants to sell, one should use "s" instead of "b".
- The order manager will work in the background to check if the order can be fulfilled. In order to check last price and status of orders you should run `fxgame> msg`.
- At any time a player can check her/his position by running `fxgame> position udacity` where `udacity` is the player's name.
- To qit the game just run `fxgame> quit`.

## Other Commands





