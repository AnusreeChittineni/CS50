from cs50 import get_int


# prompts user for credit card number
Number = get_int("What is your credit card number?\nNumber: ")

# following code is for getting the total number of digits in the number
totalDigits = 0

x = Number

while (x > 0):

    x = int(x / 10)
    totalDigits += 1
    
# checks if totalDigits is 13,15, or 16    
if totalDigits != 13 and totalDigits != 15 and totalDigits != 16:

    print("INVALID")

# following code gets every digit and stores it, starting with the last digit
# also reassigns the values of the orginal digits to another variable before making more calculations
number = []

originalnumber = []

for i in range(totalDigits):

    number.append(Number % 10)
    Number = int(Number / 10)
    originalnumber.append(number[i])
 
# following code multiplies every other digit by 2 starting from the second to last digit   
for i in range(1, totalDigits, 2):

    number[i] *= 2

# following code is used for final validation
valid = 0

if totalDigits == 13:

    # add all the produced digits togther, both the ones multipled by 2 and the ones not
    for i in range(13):
        
        sum = int((number[i] % 10) + ((number[i] / 10) % 10))
        valid += sum

    # following code checks to if the final sum is divisible by 10 and what the first digit of the number is
    if originalnumber[12] == 4 and int(valid % 10) == 0:

        print("VISA")

    else:

        print("INVALID")

if totalDigits == 15:
   
    # add all the produced digits togther, both the ones multipled by 2 and the ones not
    for i in range(15):

        sum = int((number[i] % 10) + ((number[i] / 10) % 10))
        valid += sum
    
    # following code checks to if the final sum is divisible by 10 and what the first digit  and second digit of the number are
    if originalnumber[14] == 3 and (originalnumber[13] == 4 or originalnumber[13] == 7) and int(valid % 10) == 0:

        print("AMEX")

    else:

        print("INVALID")

if totalDigits == 16:

    # add all the produced digits togther, both the ones multipled by 2 and the ones not
    for i in range(16):

        sum = int((number[i] % 10) + ((number[i] / 10) % 10))
        valid += sum
        
    # following code checks to if the final sum is divisible by 10 and what the first digit of the number is
    if originalnumber[15] == 4 and int(valid % 10) == 0:

        print("VISA")

    # following code checks to if the final sum is divisible by 10 and what the first and second digits of the number are
    elif originalnumber[15] == 5 and (originalnumber[14] <= 5)and int(valid % 10) == 0:

        print("MASTERCARD")

    else:

        print("INVALID")

