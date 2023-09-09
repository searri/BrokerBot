from flask import Flask, jsonify, request, render_template
from collections import OrderedDict
import json

# Create Flask object
app = Flask(__name__)

# Table of stock information
# OrderedDicts because years are sequential, but convenient to access data w/ key-value pairs
stocks = [
    OrderedDict({"Company": "Central City Municipal Bonds", "Y1": 100}),
    OrderedDict({"Company": "Growth Corporation of America", "Y1": 100}),
    OrderedDict({"Company": "Metro Properties, Inc.", "Y1": 100}),
    OrderedDict({"Company": "Pioneer Mutual Fund", "Y1": 100}),
    OrderedDict({"Company": "Shady Brooks Development", "Y1": 100}),
    OrderedDict({"Company": "Stryker Drilling Company", "Y1": 100}),
    OrderedDict({"Company": "Tri-City Transport Company", "Y1": 100}),
    OrderedDict({"Company": "United Auto Company", "Y1": 100}),
    OrderedDict({"Company": "Uranium Enterprises, Inc.", "Y1": 100}),
    OrderedDict({"Company": "Valley Power & Light Company", "Y1": 100}),
]

# Internal logistics
game_info = {
    "active": False,
    "current_year": 1,
    "end_years": -1,
    "main_headline": "MARKETS OPEN",
    "second_headline": "Analysts optimistic as new generation of ambitious traders hit exchange floors this morning",
    "price_changes": [0 for _ in range(10)],
}
with open("board_data.json", "r") as infile:
    board_data = json.load(infile)


@app.route("/state", methods=["GET"])
def get_state():
    """
    Make POST request here to simply return game_info dict
    """
    return jsonify(game_info)


@app.route("/start", methods=["POST"])
def start_game():
    """
    Make a POST request here to start the game
    """
    req = request.json

    if "num_years" in req:
        if not game_info["active"]:
            game_info["end_years"] = req["num_years"]
            game_info["active"] = True

        return jsonify(game_info)

    else:
        return "JSON missing attribute", 400


@app.route("/stocks", methods=["POST"])
def initialize_game_state():
    """
    Make POST requests here to play the game
    """
    reply = {"success": False, "game_info": game_info}
    req = request.json

    if "is_bull" in req and game_info["active"]:
        # Client is sending price change data
        year = "Y" + str(game_info["current_year"] + 1)
        prev_year = "Y" + str(game_info["current_year"])
        bull_or_bear = "bull" if req["is_bull"] else "bear"
        event_card = board_data["events"][req["event"] - 1]
        game_info["main_headline"] = list(board_data["events_desc"][req["event"] - 1].keys())[0]
        game_info["second_headline"] = board_data["events_desc"][req["event"] - 1][
            game_info["main_headline"]
        ]
        price_changes = [0]
        for x in board_data["bull"]:
            price_change = board_data[bull_or_bear][x][req["roll"] - 2]
            if x in event_card:
                price_change += event_card[x]
            price_changes.append(price_change)

        updated_prices = []
        for i, change in enumerate(price_changes):
            # HTML has its own way of displaying special changes, so this also needs to predict them
            if stocks[i][prev_year] >= 150:
                # Stock split
                stocks[i][year] = change + int(round(stocks[i][prev_year] / 2))
            elif stocks[i][prev_year] <= 0:
                # Stock price reset
                stocks[i][year] = 100
            else:
                stocks[i][year] = change + stocks[i][prev_year]

            updated_prices.append(stocks[i][year])
        game_info["current_year"] += 1
        reply["success"] = True
        reply["updated_prices"] = updated_prices
        game_info["price_changes"] = price_changes

        return jsonify(reply)

    else:
        return "JSON missing attribute", 400


@app.template_global()
def return_emoji(price_change):
    if price_change > 0:
        return "🔺"
    elif price_change < 0:
        return "🔻"
    else:
        return "➖"


@app.template_global()
def return_symbol(price_change):
    if price_change >= 0:
        return "+"
    else:
        return ""


@app.template_global()
def return_color_class(price_change):
    if price_change > 0:
        return "stockup"
    elif price_change < 0:
        return "stockdown"
    else:
        return ""


# This is where webpages will get directed to
@app.route("/")
def render_game():
    if game_info["active"]:
        return render_template(
            "gameactive.html",
            stock_prices=stocks,
            end=game_info["end_years"] == game_info["current_year"],
            curr_year=str(game_info["current_year"]),
            main_headline=game_info["main_headline"],
            second_headline=game_info["second_headline"],
            price_changes=game_info["price_changes"],
            zip=zip,
        )
    else:
        return render_template("gamewait.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
