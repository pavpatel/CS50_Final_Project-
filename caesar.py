import cs50
import sys

def main():
    #Error message if key isn't provided
    if len(sys.argv) != 2:
        print("Usage: python caesar.py k")
        exit(1)
    #Convert key from string to int
    k = int(sys.argv[1])
    #Error message if negative key provided
    if k < 0:
        print("Key must be a non-negative integer.")
        exit(1)
    
    #Prompt user for plaintext, begin printing ciphertext
    print("plaintext:", end=" ")
    p = cs50.get_string()
    print("ciphertext:", end=" ")
    
    #Upper and lower case determination and conversion, print non-alpha
    for i in range(len(p)):
        if p[i].islower():
            print("{}".format(chr(((((ord(p[i]) + k) - 97) % 26) + 97))), end="")
        elif p[i].isupper():
            print("{}".format(chr(((((ord(p[i]) + k) - 65) % 26) + 65))), end="")
        else: 
            print("{}".format(p[i]), end="")
    #Print new line at end
    print()
        
#Call main function
if __name__ == "__main__":
    main()