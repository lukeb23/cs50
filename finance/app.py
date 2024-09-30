import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime
from string import digits, ascii_uppercase, ascii_lowercase

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


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
    symbols = db.execute("SELECT DISTINCT symbol FROM user_transaction WHERE userID = ?", session.get("user_id"))

    # Update the price of the share currently as well as that shares profit
    for i in symbols:
        # Get the current symbol price and update it
        symbol_stats = lookup(i['symbol'])
        current_worth = symbol_stats['price']
        db.execute("UPDATE user_transaction SET current_worth = ? WHERE userID = ? AND symbol = ?",
                   current_worth, session.get("user_id"), i['symbol'])

        # Get the price the share was it was purchased at
        old_worth = db.execute("SELECT price FROM user_transaction WHERE userID = ? AND symbol = ?",
                               session.get("user_id"), i['symbol'])

        # Get the profit and update the profit the user has made
        profit = current_worth - old_worth[0]['price']

        db.execute("UPDATE user_transaction SET profit = ? WHERE userID = ? AND symbol = ?",
                   round(profit, 4), session.get("user_id"), i['symbol'])

    # Get all the data that will be used for each symbol to be displayed in the index
    symbol_data = db.execute("SELECT symbol, SUM(amount_of_shares) AS shares, current_worth, ROUND(current_worth * SUM(amount_of_shares), 2) AS currentRevenue, SUM(profit) AS totalProfit, name FROM user_transaction WHERE userID = ? AND amount_of_shares > 0 GROUP BY symbol", session.get("user_id"))

    # Get the users' total PROFIT of all the symbol's worth
    total_profit = db.execute(
        "SELECT SUM(profit) AS totalProfit FROM user_transaction WHERE userID = ?", session.get("user_id"))

    try:
        totalProfit = total_profit[0]["totalProfit"] + 1 - 1

    except TypeError:
        totalProfit = 0

    # Get the users' total REVENUE from all the symbol's worth
    total_revenue = db.execute(
        "SELECT ROUND(SUM(current_worth * amount_of_shares), 2) AS totalRevenue FROM user_transaction WHERE userID = ?", session.get("user_id"))

    totalRevenue = total_revenue[0]["totalRevenue"]

    # Get the users' current cash
    money = db.execute("SELECT cash FROM users WHERE id= ?", session.get("user_id"))

    cash = round(money[0]["cash"], 2)

    # Get the total amount of money the user has made
    try:
        total_money = cash + totalRevenue

        total_money = round(total_money, 2)

    except TypeError:

        total_money = cash

        totalRevenue = 0

    # Get the amount of shares the user has
    total_shares = db.execute(
        "SELECT SUM(amount_of_shares) AS totalShares FROM user_transaction WHERE userID = ?", session.get("user_id"))

    return render_template("index.html", symbol_data=symbol_data, totalProfit=totalProfit, totalRevenue=totalRevenue, cash=cash, total_money=total_money, total_shares=total_shares)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        try:
            symbol = request.form.get("symbol")
            shares = int(request.form.get("shares"))

        except ValueError:
            return apology("Invalid number", 400)

    #  Ensure symbol was submitted and number is real
        if not symbol:
            return apology("Missing Symbol")

        if shares < 1:
            return apology("Invalid number", 400)

        share_cost = lookup(symbol)

    #  Ensure symbol exists
        if not share_cost:
            return apology("Invalid Symbol")

        price = float(share_cost["price"])

        cost = price * shares

    # Get the amount of cash the user has and ensure its enough to buy stock
        balance = db.execute("SELECT * FROM users WHERE id = ?", session.get("user_id"))

    #  Ensure user can afford stock

        if cost > balance[0]["cash"]:
            return apology("Too Broke :/")

        new_balance = balance[0]["cash"] - cost

    # Subtract users cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, session.get("user_id"))

    # Create recording tables

        db.execute("CREATE TABLE IF NOT EXISTS user_transaction(transaction_number INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, userID INTEGER NOT NULL, symbol TEXT NOT NULL, name TEXT NOT NULL, price FLOAT NOT NULL, amount_of_shares INTEGER NOT NULL, totalCost FLOAT NOT NULL, remaining FLOAT NOT NULL, time_of_transaction TEXT NOT NULL, current_worth FLOAT NOT NULL DEFAULT 0, profit FLOAT NOT NULL DEFAULT 0)")

        db.execute("CREATE TABLE IF NOT EXISTS buy_history(transaction_number INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, userID INTEGER NOT NULL, symbol TEXT NOT NULL,  shares_bought INTEGER NOT NULL, price_bought FLOAT NOT NULL, time_of_transaction TEXT NOT NULL)")

        db.execute("INSERT INTO user_transaction(symbol, name, price, amount_of_shares, totalCost, remaining, userID, time_of_transaction) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                   symbol, share_cost["name"], price, shares, cost, new_balance, session.get("user_id"), datetime.now())

        db.execute("INSERT INTO  buy_history(userID, symbol, price_bought, shares_bought, time_of_transaction) VALUES (?, ?, ?, ?, ?)",
                   session.get("user_id"), symbol, price, shares, datetime.now())

    # Redirect to home page
        return redirect("/")
    # Else if accessed by an external link
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get transaction history
    history = db.execute("SELECT symbol, shares_sold AS shares, price_sold, time_of_transaction FROM sell_history WHERE userID = ? UNION SELECT symbol, shares_bought, price_bought, time_of_transaction FROM buy_history WHERE userID = ?", session.get(
        "user_id"), session.get("user_id"))

    # Return transaction history to html page

    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # If the page was accessed via POST (the form was submitted)
    if request.method == "POST":

        symbol = request.form.get("symbol")

    #  Ensure symbol was submitted
        if not symbol:
            return apology("Missing Symbol")

        data = lookup(symbol)

    #  Ensure symbol exists
        if not data:
            return apology("Invalid Symbol")

    # Show data on quoted html page
        return render_template("quoted.html", data=data)

    # If the page was accessed via GET
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User arrrived via Post
    if request.method == "POST":

        # Declare the variables to be added to the database
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("missing username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("missing password", 400)

        # Ensure passwords match
        elif password != confirmation:
            return apology("password's don't match", 400)

        elif len(password) < 6:
            return apology("must be longer than 6 letters", 403)

        elif numcheck(password) is False:
            return apology("password needs at least 1 number", 403)

        elif uppercheck(password) is False:
            return apology("password needs at least 1 uppercase letter", 403)

        elif lowercheck(password) is False:
            return apology("password needs at least 1 lowercase letter", 403)

        # Update users database
        hash = generate_password_hash(password)

        # Check to make sure username is unique, then update database
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) > 0:
            return apology("username taken", 400)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        # Redirect users to home page
        return redirect("/")

    # User reached route with GET
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # User got there with post
    if request.method == "POST":
        symbol = request.form.get("symbol")
        try:
            shares = int(request.form.get("shares"))

        except ValueError:
            return apology("Incorrect Shares", 400)

        if shares < 1:
            return apology("Has to be more than 1", 400)

        if not symbol:
            return apology("Missing Symbol", 400)

        data = lookup(symbol)

        current_price = float(data["price"])

        profit = current_price * shares

        # Get the users' current cash
        money = db.execute("SELECT cash FROM users WHERE id= ?", session.get("user_id"))

        # Get the users' initial share price
        old_price = db.execute("SELECT price FROM user_transaction WHERE userID = ? AND symbol = ?", session.get("user_id"), symbol)

        # Get the amount of money to subtract from old cash value

        subtracted_money = float(old_price[0]["price"]) * shares

        subtracted_cash = money[0]["cash"] - subtracted_money

        # Get users' shares and subtract them from total shares
        old_shares = db.execute(
            "SELECT SUM(amount_of_shares) AS amount_of_shares FROM user_transaction WHERE userID = ? AND symbol = ?", session.get("user_id"), str(symbol))

        if old_shares[0]["amount_of_shares"] < shares:
            return apology("No more shares of that symbol", 400)

        new_shares = old_shares[0]["amount_of_shares"] - shares

        # Update users' cash
        current_cash = subtracted_cash + profit

        # Update both tabes with correct information
        db.execute("UPDATE users SET cash = ? WHERE id = ?", current_cash, session.get("user_id"))

        time_of_transaction = db.execute(
            "SELECT time_of_transaction FROM user_transaction WHERE symbol = ? AND userID = ?", str(symbol), session.get("user_id"),)

        firstActivity = time_of_transaction[0]["time_of_transaction"]

        db.execute("UPDATE user_transaction SET amount_of_shares = 0 WHERE symbol = ? AND userID = ?", symbol, session.get("user_id"))

        db.execute("UPDATE user_transaction SET amount_of_shares = ? WHERE symbol = ? AND userID = ? AND time_of_transaction = ?",
                   new_shares, symbol, session.get("user_id"), firstActivity)

        # Record this transaction
        db.execute("CREATE TABLE IF NOT EXISTS sell_history(transaction_number INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, userID INTEGER NOT NULL, symbol TEXT NOT NULL, shares_sold INTEGER NOT NULL, price_sold FLOAT NOT NULL, time_of_transaction TEXT NOT NULL)")

        db.execute("INSERT INTO sell_history(symbol, shares_sold, price_sold, time_of_transaction, userID) VALUES (?, ?, ?, ?, ?)",
                   symbol, (shares * -1), current_price, datetime.now(), session.get("user_id"))

        return redirect("/")

    else:
        current_stocks = db.execute(
            "SELECT DISTINCT symbol FROM user_transaction WHERE userID = ? AND amount_of_shares > 0", session.get("user_id"))
        return render_template("sell.html", current_stocks=current_stocks)


# Password Checking functions for registration
def numcheck(string):
    return any(char.isdigit() for char in string)


def uppercheck(string):
    return any(char.isupper() for char in string)


def lowercheck(string):
    return any(char.islower() for char in string)