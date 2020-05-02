from flask import Flask, jsonify, request, render_template, url_for
from collections import OrderedDict
import os, ast

# Create Flask object
app = Flask(__name__)

# Table of stock information
# OrderedDicts because years are sequential, but convenient to access data w/ key-value pairs
stocks = [
    OrderedDict({"Company": "Central City Municipal Bonds", "Year 1": 100}),
    OrderedDict({"Company": "Growth Corporation of America", "Year 1": 100}),
    OrderedDict({"Company": "Metro Properties, Inc.", "Year 1": 100}),
    OrderedDict({"Company": "Pioneer Mutual Fund", "Year 1": 100}),
    OrderedDict({"Company": "Shady Brooks Development", "Year 1": 100}),
    OrderedDict({"Company": "Stryker Drilling Company", "Year 1": 100}),
    OrderedDict({"Company": "Tri-City Transport Company", "Year 1": 100}),
    OrderedDict({"Company": "United Auto Company", "Year 1": 100}),
    OrderedDict({"Company": "Uranium Enterprises, Inc.", "Year 1": 100}),
    OrderedDict({"Company": "Valley Power & Light Company", "Year 1": 100}),
]

# Internal logistics
game_info = {"active": False, "current_year": 1, "end_years": -1}

# Make POST requests here to play the game
@app.route("/stocks", methods=["POST"])
def initialize_game():
    reply = {"success": False, "game_info": game_info}
    req_data = request.data
    dict_str = req_data.decode("UTF-8")
    r = ast.literal_eval(dict_str)

    if "num_years" in r:
        # Board is attempting to start a game
        if not game_info["active"]:
            game_info["end_years"] = r["num_years"]
            game_info["active"] = True
            reply["success"] = True

    elif "price_changes" in r and game_info["active"]:
        # Board is sending price change data
        if r["year"] == game_info["current_year"] + 1:
            price_changes = r["price_changes"]
            year = "Year " + str(r["year"])
            prev_year = "Year " + str(game_info["current_year"])
            updated_prices = []
            for i in range(len(price_changes)):
                # HTML has its own way of displaying special changes, so this also needs to predict them
                if stocks[i][prev_year] >= 150:
                    # Stock split
                    stocks[i][year] = price_changes[i] + int(
                        round(stocks[i][prev_year] / 2)
                    )
                elif stocks[i][prev_year] <= 0:
                    # Stock price reset
                    stocks[i][year] = 100
                else:
                    stocks[i][year] = price_changes[i] + stocks[i][prev_year]

                updated_prices.append(stocks[i][year])
            game_info["current_year"] += 1
            reply["success"] = True
            reply["updated_prices"] = updated_prices

    return jsonify(reply)


# This is where webpages will get directed to
@app.route("/")
def start_game():
    if game_info["active"]:
        return render_template("gameactive.html", stock_prices=stocks)
    else:
        return render_template("gamewait.html")


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
