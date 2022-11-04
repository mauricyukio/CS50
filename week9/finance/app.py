import os

from sqlalchemy import create_engine, select
from sqlalchemy import Table, Column, MetaData
from sqlalchemy.sql import text
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, validate_password

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
db = 'sqlite:///finance.db'

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


@app.route("/", methods=["GET", "POST"])
@login_required
def index():
    """Show portfolio of stocks"""

    # Get user's current cash from the users table
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # POST
    if request.method == "POST":
        transaction_type = request.form.get("type")
        transaction_symbol = request.form.get("symbol")
        transaction_shares = request.form.get("shares")

        # Ensure the user has typed something in the input field
        if not transaction_shares:
            return apology("must input shares in order to continue with the transaction")

        # Ensure the value typed is an integer
        try:
            transaction_shares = int(transaction_shares)
        except:
            return apology("shares must be a whole number / integer")

        # Ensure the value typed is positive
        if transaction_shares <= 0:
            return apology("shares must be a positive number")

        # If the transaction type is a buy
        if transaction_type == "buy":

            # Lookup current price
            current_price = lookup(transaction_symbol)["price"]
            name = lookup(transaction_symbol)["name"]

            # Check if user has enough cash to perform the transaction
            if cash < current_price:
                return apology("not enough cash to perform this operation")

            # Update user's cash in the users table
            new_cash = cash - current_price * transaction_shares
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

            # Insert new transaction in the transactions table
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, name, type) VALUES(?, ?, ?, ?, ?, ?)",
                       session["user_id"], transaction_symbol, transaction_shares, current_price, name, transaction_type)

            # Redirect user to home page
            flash("Transaction Successful")
            return redirect("/")

        # If the transaction type is a sell
        if transaction_type == "sell":

            # Check if user has enough shares to perform the transaction

            current_shares = db.execute(
                "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", session["user_id"], transaction_symbol)[0]["shares"]
            if current_shares < transaction_shares:
                return apology("not enough shares to perform this transaction")

            # Lookup current price
            current_price = lookup(transaction_symbol)["price"]
            name = lookup(transaction_symbol)["name"]

            # Update user's cash in the users table
            new_cash = cash + current_price * transaction_shares
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

            # Insert new transaction in the transactions table
            db.execute("INSERT INTO transactions (user_id, symbol, shares, price, name, type) VALUES(?, ?, ?, ?, ?, ?)",
                       session["user_id"], transaction_symbol, -transaction_shares, current_price, name, transaction_type)

            # Redirect user to home page
            flash("Transaction Successful")
            return redirect("/")

    # GET
    # Get symbol, name and share sum from each stock from the transactions table
    stocks = db.execute(
        "SELECT symbol, name, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol ORDER BY symbol", session["user_id"])

    # Calculate total amount the user currently has in stocks, including cash balance
    total = cash
    for stock in stocks:
        total += stock["shares"] * lookup(stock["symbol"])["price"]

    return render_template("index.html", stocks=stocks, cash=cash, total=total, lookup=lookup, usd=usd)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # POST
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol")

        # Ensure a value for shares was submitted
        if not shares:
            return apology("must provide share amount")

        # Ensure shares is an integer
        try:
            shares = int(shares)
        except:
            return apology("amount must be an integer")

        # Ensure the amount being bought is positive
        if shares <= 0:
            return apology("amount must be positive")

        # Lookup data on the stock
        quote = lookup(symbol)

        # Ensure symbol exists
        if quote == None:
            return apology("symbol does not exist")

        # Check current user's cash amount
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Check if user has enough money to buy the amount of shares requested
        if shares * quote["price"] > current_cash[0]["cash"]:
            return apology("not enough cash for this operation")

        # Insert into the transactions table the data from the new transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, name, type) VALUES(?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, shares, quote["price"], quote["name"], "buy")

        # Update buyer's current cash in the database
        new_cash = current_cash[0]["cash"] - shares * quote["price"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        # Redirect user to home page
        flash("Transaction Successful")
        return redirect("/")

    # GET
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Get symbol, name, price, type of transaction, time of transaction and shares for each transaction from the transactions table
    stocks = db.execute(
        "SELECT type, symbol, name, shares, price, time FROM transactions WHERE user_id = ? ORDER BY time", session["user_id"])

    return render_template("history.html", stocks=stocks)


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
        # rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        meta = MetaData()
        conn = engine.connect()
        users = Table('users', meta, autoload=True, autoload_with=engine)
        s = select([users]).where(users.c.username==request.form.get("username"))
        rows = conn.execute(s).fetchall()
        conn.close()
        engine.dispose()

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        flash("Logged In")
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
    flash("Logged Out")
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # POST
    if request.method == "POST":
        symbol = request.form.get("symbol").upper()

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol")

        quote = lookup(symbol)

        # Ensure symbol exists
        if quote == None:
            return apology("symbol does not exist")

        return render_template("quoted.html", quote=quote)

    # GET
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # POST
    if request.method == "POST":
        # Forget any user_id
        session.clear()

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username")

        # Ensure password was submitted
        if not password:
            return apology("must provide password")

        # Ensure password is valid:
        if not validate_password(password):
            return apology("invalid password, check password requirements")

        # Ensure password confirmation was submitted
        if not confirmation:
            return apology("must provide password confirmation")

        # Ensure password and confirmation match
        if password != confirmation:
            return apology("password and password confirmation do not match")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        # Ensure username is not yet taken
        if len(rows) != 0:
            return apology("username already taken")

        # Register new user in the database
        user = db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username,
                          generate_password_hash(password, method='pbkdf2:sha256', salt_length=8))

        # Log new user in automatically
        session["user_id"] = user

        # Redirect user to login form
        flash("New user registered successfully! Logged in")
        return redirect("/")

    # GET
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    stocks = db.execute("SELECT DISTINCT symbol FROM transactions WHERE user_id = ?", session["user_id"])

    # POST
    if request.method == "POST":
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol")

        # Ensure a value for shares was submitted
        if not shares:
            return apology("must provide share amount")

        # Ensure shares is an integer
        try:
            shares = int(shares)
        except:
            return apology("amount must be an integer")

        # Ensure the amount being bought is positive
        if shares <= 0:
            return apology("amount must be positive")

        # Ensure user has enough shares of the stock he or she is trying to sell
        amount_owned = db.execute(
            "SELECT SUM(shares) AS shares FROM transactions WHERE user_id = ? AND symbol = ? GROUP BY symbol", session["user_id"], symbol)[0]["shares"]
        if amount_owned < shares:
            return apology("you do not own enough of this stock")

        # Lookup data on the stock
        quote = lookup(symbol)

        # Ensure symbol exists
        if quote == None:
            return apology("symbol does not exist")

        # Insert into the transactions table the data from the new transaction
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, name, type) VALUES(?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, -shares, quote["price"], quote["name"], "sell")

        # Check current user's cash amount
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])

        # Update buyer's current cash in the database
        new_cash = current_cash[0]["cash"] + shares * quote["price"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        # Redirect user to home page
        flash("Transaction Successful")
        return redirect("/")

    # GET
    return render_template("sell.html", stocks=stocks)


@app.route("/add_money", methods=["GET", "POST"])
@login_required
def add_money():
    """Add money to account"""

    # POST
    if request.method == "POST":
        money = request.form.get("money")

        # Ensure an amount was submitted
        if not money:
            return apology("must submit an amount")

        # Ensure the amount submitted is valid
        try:
            money = float(money)
        except:
            return apology("must submit a valid money amount")

        # Ensure the amount submitted is positive
        if money <= 0:
            return apology("must submit a positive money amount")

        # Check current user's cash amount
        current_cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

        # Update cash balance in the users table
        new_cash = current_cash + money
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_cash, session["user_id"])

        # Redirect user to home page
        flash("Successful")
        return redirect("/")

    # GET
    return render_template("add_money.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change current user password"""

    # POST
    if request.method == "POST":
        current_password = request.form.get("current")
        new_password = request.form.get("new")
        confirmation = request.form.get("confirmation")

        # Ensure user filled all three input fields
        if not current_password or not new_password or not confirmation:
            return apology("must fill all input fields")

        # Check if the current password is correct
        db_password = db.execute("SELECT hash FROM users WHERE id = ?", session["user_id"])

        if not check_password_hash(db_password[0]["hash"], current_password):
            return apology("current password is incorrect")

        # Ensure the new password is valid
        if not validate_password(new_password):
            return apology("invalid password, check password requirements")

        # Ensure the new password and the confirmation are the same
        if new_password != confirmation:
            return apology("password and password confirmation do not match")

        # Update the users table with the user's new password
        db.execute("UPDATE users SET hash = ? WHERE id = ?",
                   generate_password_hash(new_password, method='pbkdf2:sha256', salt_length=8), session["user_id"])

        # Redirect user to home page
        flash("Successful")
        return redirect("/")

    # GET
    return render_template("change_password.html")

if __name__ == "__main__":
    app.run(debug=True)