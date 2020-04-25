# BrokerBot
A bot player for the board game [Stocks & Bonds](https://boardgamegeek.com/boardgame/1590/stocks-bonds).

## BrokerBot's Strategy
Currently, the bot uses a simple "buy low, sell high" strategy: each turn, it sells the stocks which made over a certain threshold of money since they were purchased, and then buys the cheapest stock available. Initial portfolio is randomly selected. Areas in the code which control BrokerBot's strategy and not game rules are marked with a `//STRATEGY` comment.

## Not Yet Implemented
Will add these features if time allows; these are secondary to the main functionality.
1. Add user input through clickable rotary encoder (so BrokerBot doesn't need to use a Serial monitor through PC)
2. Add some kind of HTTP authentication
3. Use the [Google Charts API](https://developers.google.com/chart/interactive/docs/gallery/linechart) to generate a running graph of the stock prices during the game [(Example)](https://jsfiddle.net/6va5r8yq/5/)

## Wiring Diagram
![](project_assets/BrokerBot_diagram.png)