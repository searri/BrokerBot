"""
Starter code from this tutorial:
https://www.bogotobogo.com/python/python-REST-API-Http-Requests-for-Humans-with-Flask.php
"""

from flask import Flask, jsonify, request, render_template, url_for
import os

# Create Flask object
app = Flask(__name__)

# Define stock names and start prices
stocks = [
    {"name": "Central City Municipal Bonds", "price": 100},
    {"name": "Growth Corporation of America", "price": 100},
    {"name": "Metro Properties, Inc.", "price": 100},
    {"name": "Pioneer Mutual Fund", "price": 100},
    {"name": "Shady Brooks Development", "price": 100},
    {"name": "Stryker Drilling Company", "price": 100},
    {"name": "Tri-City Transport Company", "price": 100},
    {"name": "United Auto Company", "price": 100},
    {"name": "Uranium Enterprises, Inc.", "price": 100},
    {"name": "Valley Power & Light Company", "price": 100},
]

# Is there an active connection?
active_game = False

@app.route("/", methods=["GET"])
def start_game():
    if not active_game:
        return render_template("gamewait.html")
    else:
        return jsonify({"temp"})


@app.route("/quarks", methods=["GET"])
def returnAll():
    return jsonify({"quarks": quarks})


@app.route("/quarks/<string:name>", methods=["GET"])
def returnOne(name):
    theOne = quarks[0]
    for i, q in enumerate(quarks):
        if q["name"] == name:
            theOne = quarks[i]
    return jsonify({"quarks": theOne})


@app.route("/quarks", methods=["POST"])
def addOne():
    new_quark = request.get_json()
    quarks.append(new_quark)
    return jsonify({"quarks": quarks})


@app.route("/quarks/<string:name>", methods=["PUT"])
def editOne(name):
    new_quark = request.get_json()
    for i, q in enumerate(quarks):
        if q["name"] == name:
            quarks[i] = new_quark
    qs = request.get_json()
    return jsonify({"quarks": quarks})


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=80)
