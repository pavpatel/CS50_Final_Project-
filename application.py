from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    #Extract cash from table users; symbol and quantity from transactions 
    rows = db.execute("SELECT symbol ,SUM(quantity) as quantity FROM transactions WHERE id = :id GROUP BY symbol HAVING SUM(quantity) > 0", id=session["user_id"])
    c_rows = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])
    
    #Initialize list to store all data for stocks to be displayed
    stocks = []
    
    #Initialize to track total portfolio value
    total = 0
    
    #Loop through all owned symbols
    for row in rows:
        
        #Retrieve current price and name of stock
        quote = lookup(row["symbol"])
        
        #Initialize dict to hold paired values for stock
        holding = {}
        
        #Fill dict
        holding["symbol"] = row["symbol"]
        holding["name"] = quote["name"]
        holding["quantity"] = row["quantity"]
        holding["price"] = usd(quote["price"])
        holding["value"] = usd(row["quantity"] * quote["price"])
        
        #Increment total with value of stock 
        total += row["quantity"] * quote["price"]
        
        #Append data from holding dict to stocks list
        stocks.append(holding)
        
        #stocks.append(lookup(row["symbol"]))
        #stocks.append(rows[0]["quantity"])
        #value = stocks.append(quote["price"] * rows[0]["quantity"])
        
    #Add cash to total    
    total += c_rows[0]["cash"]
    
    #Render index.html; format monetary variables to USD
    return render_template("index.html", stocks=stocks, cash=usd(c_rows[0]["cash"]), total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    #If user reached route via POST
    if request.method == "POST":
        
        #Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("Must enter a stock symbol to buy!")
            
        #Ensure a quantity of shares was submitted
        if not request.form.get("shares"):
            return apology("Must enter quantity of shares to buy!")
        
        #Convert quantity of shares to int
        shares = int(request.form.get("shares"))
        
        #Ensure a positive quantity of shares was submitted
        if shares < 1:
            return apology("Must enter a positive quantity of shares to buy")
            
        #Lookup symbol and retrieve stock quote
        quote = lookup(request.form.get("symbol"))
        
        #Ensure valid stock symbol was submitted
        if quote == None:
            return apology("Must enter valid stock symbol to buy!")
            
        #Query table users for current cash
        rows = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])
        
        #Ensure user has enough cash for purchase
        if rows[0]["cash"] < (quote["price"] * shares):
            return apology("Insufficient funds for purchase!")
        
        #Record purchase in table transactions
        db.execute("INSERT INTO transactions (id, type, symbol, name, price, quantity, total) VALUES (:id, :type, :symbol, :name, :price, :quantity, :total)", 
        id=session["user_id"], type="BUY", symbol=quote["symbol"], name=quote["name"], price=quote["price"], quantity=shares, total=(quote["price"]*shares))
        
        #Update value of cash after purchase in table users
        db.execute("UPDATE users SET cash = (:cash - :total) WHERE id = :id", cash=rows[0]["cash"], total=(quote["price"]*shares), id=session["user_id"])
        
        #Redirect user to home page
        flash("Bought!")
        return redirect(url_for("index"))
        
        #Throwaway test logic
        #return render_template("test.html", cash=rows[0]["cash"], id=rows[0]["id"], price=quote["price"], symbol=quote["symbol"])
        
    #If user reached route via GET (by clicking link or via redirect)
    else:
        return render_template("buy.html")
    

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    #Extract buy/sell history from table transactions
    rows = db.execute("SELECT date ,symbol ,name ,type ,quantity ,price ,total FROM transactions WHERE id = :id ORDER BY date ,symbol ,type", id=session["user_id"])
    
    #Loop records and adjust price/total format to USD
    for row in rows:
        row["price"] = usd(row["price"])
        row["total"] = usd(row["total"])
    
    return render_template("history.html", stocks=rows)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    #If user reached route via POST, retrieve stock quote
    if request.method == "POST":
        
        #Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("Must enter a stock symbol!")
        
        #Lookup symbol and retrieve stock name, price, and symbol
        quote = lookup(request.form.get("symbol"))
        
        #Ensure valid stock symbol was submitted
        if quote == None:
            return apology("Must enter valid stock symbol!")
        
        #Display quote    
        return render_template("quoted.html", name=quote["name"], price=usd(quote["price"]), symbol=quote["symbol"])
    
    #If user reached route via GET (by clicking link or via redirect)
    else:
        return render_template("quote.html")        

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide username!")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("Must provide password!")
        
        #Ensure password confirmation was submitted
        elif not request.form.get("confirm password"):
            return apology("Must confirm password!")
        
        #Ensure both passwords match
        elif not request.form.get("password") == request.form.get("confirm password"):
            return apology("Passwords must match!")
        
        #Encrypt password
        hash = pwd_context.hash(request.form.get("password"))
        
        #Query database to see if username already exists
        result = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if result:
            return apology("That username is already in use, please pick another.")
        
        #Add user to DB in table USERS
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username=request.form.get("username"), hash=hash)
        
        #Store user ID in session to remember who is logged in
        session["user_id"] = ["id"]
        
        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    #If user reached route via POST
    if request.method == "POST":
        
        #Ensure stock symbol was submitted
        if not request.form.get("symbol"):
            return apology("Must enter a stock symbol to sell!")
            
        #Ensure a quantity of shares was submitted
        if not request.form.get("shares"):
            return apology("Must enter quantity of shares to sell!")
        
        #Convert quantity of shares to int
        shares = int(request.form.get("shares"))
        
        #Ensure a positive quantity of shares was submitted
        if shares < 1:
            return apology("Must enter a positive quantity of shares to sell")
            
        #Lookup symbol and retrieve stock quote
        quote = lookup(request.form.get("symbol"))
        
        #Ensure valid stock symbol was submitted
        if quote == None:
            return apology("Must enter valid stock symbol to sell!")
            
        #Query table transactions for owned stocks
        rows = db.execute("SELECT symbol ,SUM(quantity) AS quantity FROM transactions WHERE id = :id AND symbol = :symbol GROUP BY symbol", 
        id=session["user_id"], symbol=request.form.get("symbol"))
        
        #Ensure user owns shares of stock to sell
        if not rows:
            return apology("Must enter a stock you own to sell!")
        
        if shares > rows[0]["quantity"]:
            return apology("You don't own that many shares to sell!")
        
        #Record sale in table transactions
        db.execute("INSERT INTO transactions (id, type, symbol, name, price, quantity, total) VALUES (:id, :type, :symbol, :name, :price, :quantity, :total)", 
        id=session["user_id"], type="SELL", symbol=quote["symbol"], name=quote["name"], price=quote["price"], quantity=(-shares), total=(quote["price"]*shares))
        
        
        #Update value of cash after sale in table users
        db.execute("UPDATE users SET cash = (cash + :total) WHERE id = :id", total=(quote["price"]*shares), id=session["user_id"])
        
        #Redirect user to home page
        flash("Sold!")
        return redirect(url_for("index"))
        
    #If user reached route via GET (by clicking link or via redirect)
    else:
        return render_template("sell.html")

@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash():
    """Add cash to account"""
    
    #If user reached route via POST
    if request.method == "POST":
        
        #Ensure a dollar amount was submitted
        if not request.form.get("amount"):
            return apology("Must enter USD amount to add to account!")
        
        #Convert quantity of shares to integer (intentional dropping of cents)
        amount = int(request.form.get("amount"))
        
        #Ensure positive dollar amount was submitted; minimum $1
        if amount < 1:
            return apology("Must enter amount of least $1!")
        
        #Update value of cash in table users
        db.execute("UPDATE users SET cash = (cash + :amount) WHERE id = :id", id=session["user_id"], amount=amount)
        
        #Record addition of cash in table transactions
        db.execute("INSERT INTO transactions (id, type, symbol, name, price, quantity, total) VALUES (:id, :type, :symbol, :name, :price, :quantity, :total)",
        id=session["user_id"], type="ADD CASH", symbol="USD", name="Cash Transfer to Account", price=int(0), quantity=int(0), total=amount)
        
        flash("Cash Added!")
        return redirect(url_for("index"))
    
    #If user reached route via GET (by clicking link or via redirect)
    else: 
        return render_template("cash.html")
