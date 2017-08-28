import cs50
import re

from helpers import *

#Configure application
app = Flask(__name__)
app.secret_key = "sUper_$ecr3t" #Ref: http://flask.pocoo.org/docs/0.11/patterns/flashing/
JSGlue(app)

#Ensure responses aren't cached since CCN is sensitive data
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

@app.route("/", methods=["GET", "POST"])
def index():

    #If user reached route via POST
    if request.method == "POST":

        #Flash error message and return apology if no CCN submitted
        if not request.form.get("ccn"):
            flash("Uh-oh looks like you forgot something!")
            return apology("Yeah... I'm going to need you to enter a credit card number to validate mmmkay?")

        #Remove CCN mask (hyphens), store CCN and string length for later use
        #Ref: https://stackoverflow.com/questions/22520932/python-remove-all-non-alphabet-chars-from-string (adjusted for numeric)
        regex = re.compile("[^0-9]")
        ccn = regex.sub("", request.form.get("ccn"))
        ccn_length = len(ccn)

        #Flash error message and return apology if unsupported or truncated CCN submitted
        if ccn_length not in (12,13,14,15,16,19):
            flash("Uh-oh looks like you forgot some digits!")
            return apology("Yeah... I'm going to need you to enter a full credit card number with 12-16 or 19 digits mmmkay?")

        #Initialize list
        luhn = list()

        #Loop CCN and add each digit to list as int
        for i in range (ccn_length):
            luhn.append(int(ccn[i]))

        #Store check digit
        check = luhn[ccn_length-1]

        #Luhn Step 1: Remove check digit from list (CCN); ccn_length updated accordingly for later use
        del luhn[ccn_length-1]
        ccn_length -= 1

        #Luhn Step 2: Reverse digits in CCN
        luhn.reverse()

        #Luhn Step 3: Multiply every other digit by 2 and subtract 9 if product > 9
        for j in range (ccn_length):
            if j % 2 == 0:
                luhn[j] = luhn[j] * 2
                if luhn[j] > 9:
                    luhn[j] = luhn[j] - 9

        #Luhn Step 4: Add digits together
        total = sum(luhn)

        #Luhn Step 5: Add total and check digit, mod by 10, flash validity determination
        if (total + check) % 10 == 0:
            flash("Valid!", "success")
        else:
            flash("Invalid!", "error")

        #Redirect to index
        return redirect(url_for("index"))

    #If user reached route via GET
    else:
        return render_template("index.html")