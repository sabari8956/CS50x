import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

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
    #  gets the stock data of the user
    user_data = db.execute("SELECT stock, SUM(stock_count) AS stock_count FROM transactions WHERE user_id= ?  GROUP BY stock HAVING SUM(stock_count) > 0",session["user_id"] )

    # total money owned by the user

    total = 0

    for data in user_data:
        # looking up the stock data
        stock_data = lookup(data['stock'].upper())
        data['price'] = stock_data['price']
        data['total'] = round(data['stock_count'] * data['price'], 2) # rounding can be added
        total += data['total']

    # getting the balance of the user as on in cash
    user_balance = db.execute("SELECT cash FROM users WHERE id = ? ", session["user_id"])[0]
    total += user_balance['cash']

    #  rendering and passing data to jinja

    return render_template("index.html", datas= user_data , cash = round(user_balance['cash'], 2), total= round(total, 2))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    #  if post / comes from clicking buy button
    if request.method == "POST":
        # gets the value
        stock_name = request.form.get('symbol')
        stock_quantity = request.form.get('shares')

        #  checks if None
        if stock_name == None or stock_quantity == None:
            return apology("Invalid form data")

#
        if not stock_quantity.isdigit() or int(stock_quantity) <= 0 :
            return apology("Enter a Valid No of Stock")

        # looks up the stock details
        stock_data = lookup(stock_name.upper())

        # if stock found

        if stock_data != None:
            # quote if a stock costs 150 then 10 stocks = $ 1500
            stock_quote = stock_data['price'] * int(stock_quantity)

            # gets the balance cash of the user
            balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
            if balance is None :
                return apology("balance not found")

            balance = balance[0]['cash']

            # if he has cash to afford it
            if balance >= stock_quote:
                # buying stock
                cur_balance = balance - stock_quote
                s_name, s_price = stock_data["name"],stock_data["price"]

                try:
                    # making transactions
                    # to overcome race conditions
                    # db.execute("BEGIN")
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", cur_balance, session["user_id"])
                    db.execute("INSERT INTO transactions(user_id, type, stock, stock_count, cost_bought) VALUES(?,'buy', ?, ?, ?) "
,session["user_id"], s_name, int(stock_quantity), s_price )
                    # db.execute("COMMIT")
                    flash("Bought")
                    return redirect("/")

                except:

                    # db.execute("ROLLBACK")
                    return apology("Some Error occurred")

            #  if the quote is higher the balance
            return apology("low balance")

        # stock name not found
        else:
            return apology("no stock found")


    return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    stock_history = db.execute("SELECT type, stock, stock_count, time FROM transactions WHERE user_id = ? ORDER BY time DESC", session["user_id"])



    return render_template('history.html', transactions= stock_history )


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
    if request.method == "POST":
        # gets symbol value
        symbol = request.form.get('symbol')

        # if the user didnt type any value
        if not symbol:
            return apology("enter a valid stock")

        # getting data/value of the stock
        stock_data = lookup(symbol.upper())
        # if stock exists
        if stock_data :
            return render_template("quoted.html", name= stock_data["name"], price=stock_data["price"], symbol= stock_data["symbol"])

        # if not stock found with the symbol
        return apology("Stock data not found")

    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # gets the username
        username = request.form.get('username')

        # if the username doesnt have any value
        if not username:
            return apology("enter a username")

        # checking if user exits
        val = db.execute("SELECT count(username) AS CNT FROM users WHERE username = ?", username)
        val = val[0]['CNT']

        if (val != 0):
            # an user user exits whith username
            return apology("User exists", code=400)

        # gets password
        password_1 = request.form.get('password')
        password_2 = request.form.get('confirmation')

        # if the user typed anything in password
        if not password_1 or not password_2:
            return apology("Enter password")

        # checks if both passwords are equal
        if password_1 != password_2:
            return apology("Incorrect password")

        # hashing password
        hashed_password = generate_password_hash(password_1)

        # add record into table
        db.execute("BEGIN")
        db.execute('INSERT INTO users (username , hash) VALUES (?, ?)', username, hashed_password)
        session["user_id"] = int(db.execute("SELECT id FROM users WHERE username = ?", username)[0]['id'])
        db.execute("COMMIT")

        # redirecting value to "/" aka landing page
        return redirect("/")

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # get the stocks the user owns
    stocks = db.execute("SELECT stock, SUM(stock_count) AS stock_count FROM transactions WHERE user_id= ? GROUP BY stock ",session["user_id"])
    # formats the data
    stock_owned = {}
    for stock in stocks:
        stock_name = stock['stock']
        stock_owned[stock_name] = stock['stock_count']


    if request.method == "POST":
        stock_symbol = request.form.get('symbol')
        no_shares = int(request.form.get('shares'))

        # if the user did not select a stock or
        # the user can edit the values of the option by going into dev tools and changing it

        # to overcome we can save all the stocks owned by user in a list
        # and compare it, and we dont need to check for stock_symbol == "None" coz it wont be there in list

        # we can add the if the lookup was unsucessful return apology

        stock_data = lookup(stock_symbol.upper())

        if stock_symbol not in stock_owned or stock_data == None:
            return apology("Select a Valid stock")

        # if user enters more share than he has
        if stock_owned[stock_symbol] < no_shares:
            return apology("You dont have that much shares")

        stock_quote = no_shares * stock_data["price"]

        db.execute("BEGIN")
        db.execute("INSERT INTO transactions(user_id, type, stock, stock_count, cost_bought) VALUES(?, 'sell', ?, ?, ?)"
                   ,session["user_id"], stock_symbol, -no_shares, stock_data["price"])
        db.execute("UPDATE users SET cash = cash + ? WHERE ID = ?", stock_quote, session["user_id"])
        db.execute("COMMIT")
        flash("Sold")
        return redirect("/")



    return render_template('sell.html', stocks=stock_owned)
