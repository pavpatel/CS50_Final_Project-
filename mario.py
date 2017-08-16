import cs50


def main():
    height = get_height()
    #Loop for printing each row
    for i in range(height):
        #Print spaces, then hashes, on each row with no seperator
        print(' ' * ((height - 1) - i), '#' * (height - (height - 2) + i), sep='')
    
#Prompt user for height
def get_height():
    #Repeat until integer provided between 0 - 23
    while True:
        print("Enter a number between 0 and 23:", end=" ")
        h = cs50.get_int()
        if h >= 0 and h <= 23:
            break
    #Return height to main function
    return h

#def spaces(height, i):
    #print('*' * ((height - 1) - i))
    
#def hashes(height, spaces, i):
    #print('#' * ((height + i)))

#Call main function
if __name__ == "__main__":
    main()