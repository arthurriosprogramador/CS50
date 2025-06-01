import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

GET = "GET"
POST = "POST"

PURCHASE = "PURCHASE"
SALES = "SALES"

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    id = session["user_id"]
    purchase_transactions = db.execute("""SELECT symbol, SUM(shares) as shares
                              FROM transactions
                              WHERE user_id = ? AND transaction_type = ?
                              GROUP BY symbol""", id, PURCHASE)
    sale_transactions = db.execute("""SELECT symbol, SUM(shares) as shares
                              FROM transactions
                              WHERE user_id = ? AND transaction_type = ?
                              GROUP BY symbol""", id, SALES)
    user_cash = db.execute("SELECT cash FROM users WHERE id = ?", id)[0]["cash"]
    stocks = []
    total = user_cash
    for purchase in purchase_transactions:
        sales = next((sale for sale in sale_transactions if sale["symbol"] == purchase["symbol"]), None)
        owned_shares = purchase["shares"] - (sales["shares"] if sales else 0)
        if owned_shares > 0:
            price = lookup(purchase["symbol"])["price"]
            share_total = price * purchase["shares"]
            stocks.append(
                {
                    "symbol": purchase["symbol"],
                    "shares": owned_shares,
                    "price": usd(price),
                    "share_total": usd(share_total)
                }
            )
            total += share_total


    return render_template("index.html", stocks=stocks, cash=usd(user_cash), total=usd(total))


@app.route("/buy", methods=[GET, POST])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == POST:
        id = session["user_id"]
        response = None
        symbol = str(request.form.get("symbol")).strip().upper()
        shares = 0
        cash = db.execute("SELECT cash FROM users WHERE id = ?", id)[0]["cash"]

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Invalid number of shares")

        if not symbol:
            return apology("Must provide symbol", 400)
        elif shares < 1:
            return apology("Must provide a number of shares", 400)
        else:
            try:
                response = lookup(symbol)
                if response is None:
                    return apology("Something went wrong. Verify if the typed symbol is correct", 400)
            except:
                return apology("Something went wrong", 500)
            else:
                price = float(response["price"]) * shares
                if price > cash:
                    return apology("You don't have enough money for this operation")
                else:
                    try:
                        db.execute("INSERT INTO transactions (user_id, symbol, transaction_type, price, shares) VALUES(?, ?, ?, ?, ?)", id, symbol, PURCHASE, response["price"], shares)
                    except:
                        db.execute("ROLLBACK;")
                        return apology("Something went wrong", 500)
                    else:
                        db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash - price), id)
                    return redirect("/")
    else:
        return render_template("buy.html")


@app.route("/history", methods=[GET, POST])
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    transactions = []
    transactions_formatted = []
    symbols = set()
    try:
        transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id)
    except:
        return apology("Something went wrong", 500)
    else:
        for transaction in transactions:
            total = transaction["price"] * transaction["shares"]
            formatted_date = datetime.strptime(transaction["timestamp"], "%Y-%m-%d %H:%M:%S").strftime("%B %-d, %Y %-I:%M %p")
            transactions_formatted.append(
                {
                    "symbol": transaction["symbol"],
                    "price": usd(transaction["price"]),
                    "shares": transaction["shares"],
                    "transaction_type": str(transaction["transaction_type"]).lower().capitalize(),
                    "timestamp": formatted_date,
                    "total": usd(total)
                }
            )
            symbols.add(transaction["symbol"])

        max_price = max(transaction["price"] * transaction["shares"] for transaction in transactions) * 1.1
        min_price = min(transaction["price"] * transaction["shares"] for transaction in transactions)
        step_price = min_price * 2

        if request.method == POST:
            filter_symbol = request.form.get("symbol")
            filter_transaction_type = request.form.get("transaction-type")
            filter_max_price = request.form.get("max-price")
            filtered_transactions = transactions_formatted

            if filter_symbol and filtered_transactions:
                filtered_transactions = [transaction for transaction in filtered_transactions if transaction["symbol"] == filter_symbol]
            if filter_transaction_type and filtered_transactions:
                filtered_transactions = [transaction for transaction in filtered_transactions if str(transaction["transaction_type"]).lower() == filter_transaction_type]
            if filter_max_price and filtered_transactions:
                filtered_transactions = [transaction for transaction in filtered_transactions if float(transaction["total"].replace("$", "").replace(",", "")) <= float(filter_max_price)]

            return render_template(
                "history.html",
                transactions=filtered_transactions,
                symbols=symbols,
                min_price_range=min_price,
                max_price_range=max_price,
                step_price_range=step_price
                )
        else:
            return render_template(
                "history.html",
                transactions=transactions_formatted,
                symbols=symbols,
                min_price_range=min_price,
                max_price_range=max_price,
                step_price_range=step_price
                )


@app.route("/login", methods=[GET, POST])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == POST:
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=[GET, POST])
@login_required
def quote():
    """Get stock quote."""
    if request.method == POST:
        response = None
        symbol = str(request.form.get("symbol")).strip()
        if not symbol:
            return apology("Must provide a symbol")
        else:
            try:
                response = lookup(symbol)
                if response is None:
                    return apology("Something went wrong. Verify if the typed symbol is correct", 400)
            except:
                return apology("Something went wrong", 500)
            else:
                return render_template("quoted.html", name=response["name"], price=usd(response["price"]), symbol=response["symbol"])

    else:
        return render_template("quote.html")


@app.route("/register", methods=[GET, POST])
def register():
    """Register user"""
    if request.method == POST:
        name = str(request.form.get("username")).strip()
        password = str(request.form.get("password")).strip()
        confirmation = str(request.form.get("confirmation")).strip()
        if not name:
            return apology("Must provide username", 400)
        elif not password or not confirmation:
            return apology("Must provide a password and confirm correctly", 400)
        elif password != confirmation:
            return apology("Password and confirmation should be equal", 400)
        else:
            try:
                hash = generate_password_hash(password)
                db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", name, hash)
            except:
                return apology("There's already an username registered")
            else:
                return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=[GET, POST])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]
    if request.method == POST:
        chosen_symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        purchased_shares = db.execute("""SELECT SUM(shares) as shares
                                  FROM transactions
                                  WHERE user_id = ?
                                  AND symbol = ? AND transaction_type = ?""", user_id, chosen_symbol, PURCHASE)[0]["shares"]
        sold_shares = db.execute("""SELECT symbol, SUM(shares) as shares
                              FROM transactions
                              WHERE user_id = ? AND symbol = ? AND transaction_type = ?""", user_id, chosen_symbol, SALES)[0]["shares"]

        owned_shares = purchased_shares - (sold_shares if sold_shares else 0)
        if not chosen_symbol:
            return apology("Must choose a stock", 400)
        elif shares < 1:
            return apology("Must provide a number of shares", 400)
        elif shares > owned_shares:
            return apology("You do not have enough shares", 400)
        else:
            current_price = lookup(chosen_symbol)["price"]
            selling_price = current_price * shares
            current_cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
            try:
                db.execute("BEGIN;")
                db.execute("UPDATE users SET cash = ? WHERE id = ?", (current_cash + selling_price), user_id)
                db.execute("INSERT INTO transactions (user_id, symbol, transaction_type, price, shares) VALUES(?, ?, ?, ?, ?)", user_id, chosen_symbol, SALES, current_price, shares)
                db.execute("COMMIT;")
            except:
                db.execute("ROLLBACK;")
                return apology("Something went wrong", 400)
            else:
                return redirect("/")
    else:
        symbols = db.execute("SELECT DISTINCT symbol FROM transactions WHERE user_id = ?", user_id)

        return render_template("sell.html", symbols=symbols)


@app.route("/deposit", methods=[GET, POST])
@login_required
def deposit():
    if request.method == POST:
        deposit_cash = request.form.get("cash")
        if not deposit_cash:
            return apology("Must provide a value", 400)
        else:
            try:
                db.execute("UPDATE users SET cash = cash + ?", float(deposit_cash))
            except:
                return apology("Something went wrong", 500)
            else:
                return redirect("/")
    else:
        return render_template("deposit.html")
