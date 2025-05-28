import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session

GET = "GET"
POST = "POST"

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=[GET, POST])
def index():
    if request.method == POST:
        session["name"] = request.form.get("name")
        session["month"] = request.form.get("month")
        session["day"] = request.form.get("day")
        db.execute("INSERT INTO birthdays (name, month, day) VALUES(?, ?, ?)", session["name"], session["month"], session["day"])

        return redirect("/")
    else:

        birthdays = db.execute("SELECT * FROM birthdays")

        return render_template("index.html", birthdays=birthdays)


@app.route("/delete", methods=[POST])
def delete():
    id = request.form.get("id")
    db.execute("DELETE FROM birthdays WHERE id = ?", id)
    return redirect("/")

@app.route("/update", methods=[POST])
def update():
    id = request.form.get("id")
    name = request.form.get("name")
    month = request.form.get("month")
    day = request.form.get("day")
    db.execute("UPDATE birthdays SET name = ?, month = ?, day = ? WHERE id = ?", name, month, day, id)
    return redirect("/")
