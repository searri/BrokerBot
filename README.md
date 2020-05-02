# BrokerBot
A bot player for the board game [Stocks & Bonds](https://boardgamegeek.com/boardgame/1590/stocks-bonds).

## BrokerBot's Strategy
Currently, the bot uses a simple "buy low, sell high" strategy: each turn, it sells the stocks which made over a certain threshold of money since they were purchased, and then buys the cheapest stock available. Initial portfolio is randomly selected. Areas in the code which control BrokerBot's strategy and not game rules are marked with a `//STRATEGY` comment.

## Requirements
Arduino code has the following dependencies:
- `ArduinoJson`: obtainable through library manager
- ESP8266 Boards: follow [this tutorial](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

Additionally, you will need to have `connection.h` visible to Arduino, which contains Internet credentials:
```c
// Your WiFi's name
#define WIFI_NAME "Name"

// Your WiFi's passowrd
#define WIFI_PSWD "Password"

// Address to make HTTP requests to
#define GAMESERVER_IP "http://<some ip>/stocks"
```

## Not Yet Implemented
Will add these features if time allows; these are secondary to the main functionality.
1. Add some kind of HTTP authentication
2. Use the [Google Charts API](https://developers.google.com/chart/interactive/docs/gallery/linechart) to generate a running graph of the stock prices during the game [(Example)](https://jsfiddle.net/6va5r8yq/5/)

## Wiring Diagram
![](project_assets/BrokerBot_diagram.png)
