from flask import Flask, jsonify, request, render_template, url_for
from collections import OrderedDict
import os

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
active_game = False
current_year = 1

# This is where webpages will get directed to
@app.route("/")
def start_game():
    if not active_game:
        return render_template("gamewait.html")
    else:
        return render_template("gameactive.html", stock_prices=stocks)


# TODO: Implement POST requests to update stock prices/start game
@app.route("/quarks", methods=["POST"])
def addOne():
    new_quark = request.get_json()
    quarks.append(new_quark)
    return jsonify({"quarks": quarks})


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
