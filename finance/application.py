import os
import datetime

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# API_KEY=pk_7f3aefebb590458592ef950540652079

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    grandtotal = 0

    number_of_records = db.execute("SELECT count(*) as count FROM portfolio WHERE id = :id", id = session["user_id"])

    if number_of_records[0]["count"] > 0:

        stocks = db.execute("SELECT * FROM portfolio WHERE id = :id", id = session["user_id"])

        for stock in stocks:
            symbol = str(stock["symbol"])
            shares = int(stock["shares"])
            name = str(stock["name"])
            quote = lookup(symbol)
            current_price = float(quote['price'])
            total_value_share = float(current_price * shares)
            grandtotal += total_value_share
            db.execute("UPDATE portfolio SET price=:price, total =:total WHERE id=:id AND symbol=:symbol",
                        price = current_price, total = total_value_share, id = session["user_id"], symbol=symbol)

        rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])

        cash = rows[0]["cash"]

        return render_template("index.html", stocks = stocks, cash = cash, grandtotal = grandtotal)

    else:

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        cash = rows[0]["cash"]

        return render_template("index.html", cash = cash, grandtotal = grandtotal)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol")

        elif not request.form.get("shares"):
            return apology("must provide number of shares")

        shares = int(request.form.get("shares"))

        if shares < 1:
            return apology("must enter a number greater than 0")

        symbol = request.form.get("symbol")

        stock = lookup(symbol)

        if stock == None:
            return apology("Stock does not exist")

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        # How much $$$ the user still has in their account
        cash_remaining = rows[0]["cash"]
        price_per_share = stock["price"]

        # Calculate the price of requested shares
        total_price = price_per_share * shares


        if total_price > cash_remaining:

            return apology("you do not have enough cash to make this purchase")

        # select user shares of symbol
        user_shares = db.execute("SELECT shares FROM portfolio WHERE id = :id AND symbol = :symbol", \
                                id = session["user_id"], symbol = stock["symbol"])

        db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash = cash_remaining - total_price, id=session["user_id"] )

        #if user doesn't already have that stock then create a new object
        if not user_shares:
            db.execute("INSERT INTO portfolio (name, shares, price, total, symbol, id) \
                        VALUES(:name, :shares, :price, :total, :symbol, :id)", \
                        name=stock["name"], shares= shares, price= price_per_share, \
                        total=  usd(total_price), symbol= stock["symbol"], id= session["user_id"])

        #if user already has it then increase number of shares
        else:
            shares_total = int(user_shares[0]["shares"]) + shares
            db.execute("UPDATE portfolio SET shares=:shares \
                        WHERE id=:id AND symbol=:symbol", shares = shares_total, \
                        id=session["user_id"], symbol = stock["symbol"])

        timestamp = datetime.datetime.now()

        db.execute("INSERT INTO transactions (id, symbol, name, shares, price, timestamp) \
                    VALUES (:id, :symbol, :name, :shares, :price, :timestamp)", \
                    id = session["user_id"], symbol = stock["symbol"], name = stock["name"],
                    shares = shares, price = price_per_share, timestamp = timestamp)

        return redirect("/")

    else:

        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    histories = db.execute("SELECT * from transactions WHERE id=:id", id=session["user_id"])

    for history in histories:
            symbol = str(history["symbol"])
            shares = int(history["shares"])
            name = str(history["name"])
            price = float(history["price"])
            timestamp = history["timestamp"]

    return render_template("history.html", histories = histories)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change user password"""

    if request.method == "POST":

        if not request.form.get("old_password"):
            return apology("must provide old password", 403)

        elif not request.form.get("new_password"):
            return apology("must provide  new password", 403)

        elif not request.form.get("new_password_confirmation"):
            return apology("must provide new password confirmation", 403)

        old_password = request.form.get("old_password")

        new_password = request.form.get("new_password")

        password_confirmation = request.form.get("new_password_confirmation")

        stored_password = db.execute("SELECT hash FROM users WHERE id = :id", id = session["user_id"])

        # Ensure passwords are matching
        if new_password != password_confirmation:
            return apology("new password didn't match", 403)

        elif old_password == new_password:
            return apology("new password cannot be the same as old password", 403)

        new_password_hash = generate_password_hash(new_password)

        db.execute("UPDATE users SET hash = :hash WHERE id = :id", id = session["user_id"], hash = new_password_hash)

        return redirect("/")

    else:

        return render_template("change_password.html")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide symbol")

        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        if quote == None:
            return apology("Stock does not exist")

        return render_template("quote_display.html", displayquote = quote)

    else:

        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # Ensures password confirmation was submitted
        elif not request.form.get("password_confirmation"):
            return apology("must provide password confirmation")

        # Ensure passwords are matching
        elif request.form.get("password") != request.form.get("password_confirmation"):
            return apology("password didn't match")

       # hash the password and insert a new user in the database
        hash_password = generate_password_hash(request.form.get("password"))

        new_user = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)",
                              username = request.form.get("username"), hash = hash_password)

        # checks if username is unique
        if not new_user:
            return apology("username taken")

        rows = db.execute("SELECT * FROM users WHERE username = :username",
                  username = request.form.get("username"))

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:

        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        shares = int(request.form.get("shares"))

        stock = db.execute("SELECT * FROM portfolio WHERE symbol = :symbol AND id = :id",
                            symbol = symbol, id = session["user_id"])

        stock = db.execute("SELECT SUM(shares) as total_shares FROM portfolio WHERE id = :id AND symbol = :symbol GROUP BY symbol",
                           id=session["user_id"], symbol=request.form.get("symbol"))

        if stock[0]["total_shares"] < int(shares):

            return apology("you can't sell more than you own", 400)

        else:

            rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

            cash = rows[0]["cash"]

            price_per_share = quote["price"]

            total_price =  price_per_share * float(shares)

            # update user cash (increase)
            db.execute("UPDATE users SET cash = :cash + :price WHERE id = :user_id", cash = cash, price = total_price, user_id = session["user_id"])

            # decrement the shares count
            shares_total = stock[0]["total_shares"] - int(shares)

            # if after decrement is zero, delete shares from portfolio
            if shares_total == 0:

                db.execute("DELETE FROM portfolio \
                        WHERE id=:id AND symbol=:symbol", \
                        id = session["user_id"], \
                        symbol = symbol)

            # otherwise, update portfolio shares count
            else:

                db.execute("UPDATE portfolio SET shares=:shares \
                    WHERE id=:id AND symbol=:symbol", \
                    shares = shares_total, id = session["user_id"], \
                    symbol = symbol)

            name = db.execute("SELECT name FROM portfolio WHERE id = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol)

            timestamp = datetime.datetime.now()

            db.execute("INSERT INTO transactions (id, symbol, name, shares, price, timestamp) \
                       VALUES(:id, :symbol, :name, :shares, :price, :timestamp)", \
                       symbol = symbol, shares = -shares, name = quote["name"], \
                       price = price_per_share, id = session["user_id"], timestamp = timestamp)

            # Redirect user to home page
            return redirect("/")

    else:

        stocks = db.execute("SELECT symbol, SUM(shares) as total_shares FROM portfolio WHERE id = :id GROUP BY symbol HAVING total_shares > 0", id=session["user_id"])

        return render_template("sell.html", stocks=stocks)

def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
