import cs50

#Determine change by dividing amount with largest coin and repeating for remainder using all smaller coins
def main():
    amt = get_amt()
    quarter = round(amt // 25)
    amt = round(amt % 25)
    dime = round(amt // 10)
    amt = round(amt % 10)
    nickel = round(amt // 5)
    amt = round(amt % 5)
    penny = round(amt // 1)
    #Print total number of coins required for change
    print("{}".format(quarter + dime + nickel + penny))
    
#Prompt user for amount    
def get_amt():
    #Repeat until non-negative value provided
    while True:
        print("O hai! How much change is owed? $", end=" ")
        amt = cs50.get_float()
        if amt >= 0:
            break
    #Multiply amount by 100 to convert dolalrs to cents
    amt = amt * 100
    #Return amount to main function
    return amt

#Call main function    
if __name__ == "__main__":
    main()