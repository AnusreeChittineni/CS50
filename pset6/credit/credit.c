#include <stdio.h>
#include <cs50.h>

int main(void)
{  
    //prompts user for credit card number
    long Number = get_long("What is your credit card number?\nNumber:");
  
    //following code is for getting the total number of digits in the number
    int totalDigits = 0;

    long x = Number;

    while (x > 0)
    {
        x = x / 10;
        totalDigits ++;
    }  
  
    //checks if totalDigits is 13,15, or 16
    if ((totalDigits != 13) && (totalDigits != 15) && (totalDigits != 16))
    {
        printf("INVALID\n");
    }

    //following code gets every digit and stores it, starting with the last digit  
    int number[totalDigits];
      
    for (int i = 0; i < totalDigits; i++)
    {
        number[i] = Number % 10;
        Number  = Number / 10;
    }
    
    //following code reassigns the values of the orginal digits to another variable
    int originalnumber[totalDigits];

    for (int i = 1; i < totalDigits; i++)
    {
        originalnumber[i] = number[i];
    }
    
    //following code multiplies every other digit by 2 starting from the second to last digit
    for (int i = 1; i < totalDigits; i += 2)
    {
        number[i] = number[i] * 2;
    }
    
    int valid = 0;

    int sum;

    if (totalDigits == 13)
    {
        //following code is used to add all the produced digits togther, both the ones multipled by 2 and the           ones not
        for (int i = 0; i < totalDigits; i++)
        {
            sum = (number[i] % 10) + (number[i] / 10 % 10);
            valid = valid + sum;
        }
        //following code checks to if the final sum is divisible by 10 and what the first digit of the number is
        if (originalnumber[12] == 4 && valid % 10 == 0)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    if (totalDigits == 15)
    {
        //following code is used to add all the produced digits togther, both the ones multipled by 2 and the           ones not
        for (int i = 0; i < totalDigits; i++)
        {
            sum = (number[i] % 10) + (number[i] / 10 % 10);
            valid = valid + sum;
        }
        //following code checks to if the final sum is divisible by 10 and what the first and second digits of          the number are
        if (originalnumber[14] == 3 && valid % 10 == 0 && (originalnumber[13] == 4 || originalnumber[13] == 7))
        {
            printf("AMEX\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    if (totalDigits == 16)
    {
        //following code is used to add all the produced digits togther, both the ones multipled by 2 and the           ones not
        for (int i = 0; i < totalDigits; i++)
        {
            sum = (number[i] % 10) + (number[i] / 10 % 10);
            valid = valid + sum;
        }
        //following code checks to if the final sum is divisible by 10 and what the first digit of the number is
        if (originalnumber[15] == 4 && valid % 10 == 0)
        {
            printf("VISA\n");
        }
        //following code checks to if the final sum is divisible by 10 and what the first and second digits of          the number are
        else if (originalnumber[15] == 5 && valid % 10 == 0 && (originalnumber[14] <= 5))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }  
} 