Overview:

My final project for CS50 is a program that validates a given credit card number (CCN) using the Luhn Algorithm.

------------------------------------------------------------------------------------------------------------------------------------

Key Notes:

1) The program can validate CCNs that range from 12-16 or 19 digits total.
2) CCNs will always be masked to format, "0000-0000-0000-0000-000" regardless of length.
3) Users need not worry about entering hyphens or spaces as masking is automatic and non-numeric characters are restricted.
4) The Luhn validity determination will be flashed in the header for three seconds.
5) See comments within each file for documentation references.

------------------------------------------------------------------------------------------------------------------------------------

Examples for Demo:

CCN                      Luhn Determination*     Digits
502069683675			 Valid                  (12)
4840241779516			 Valid                  (13)
30394512193005			 Valid                  (14)
371449635398431 		 Valid                  (15)
4826197647389731		 Valid                  (16)
3532247252533058490		 Valid                  (19)
501883075010			 Invalid                (12)

*Only one Invalid CCN is provided as any of the Valid CCNs above can be made Invalid by changing the last digit.

------------------------------------------------------------------------------------------------------------------------------------

Package Includes:

1) README.txt
2) application.py
3) helpers.py
4) static/scripts.js
5) static/styles.css
6) templates/apology.html
7) templates/index.html
8) templates/layout.html