# BrokerBot
A bot player for the board game [Stocks & Bonds](https://boardgamegeek.com/boardgame/1590/stocks-bonds).

## Implementation
Currently, the bot uses a simple "buy low, sell high" strategy: each turn, it sells the stocks which made over a certain threshold of money since they were purchased, and then buys the cheapest stock available. Initial portfolio is randomly selected. Areas in the code which control BrokerBot's strategy and not game rules are marked with a `//STRATEGY` comment.
