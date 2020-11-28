#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    //following code checks the conditions on the key

    //checks to ensure there was only one command line-argument given
    if (argc != 2)
    {
        printf("Program accepts a single command-line argument\n");
        return 1;
    }

    //checks to ensure the length of the key is 26 characters
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 alaphabetic characters.\n");
        return 1 ;
    }

    //checks to ensure all characters are alphabetic characters
    else
    {
        //loops through every character and checks if it is an alphabetic character
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (isalpha(argv[1][i]) ==  0)
            {
                printf("Key must only inlcude alphabetic characters.\n");
                return 1;
            }

        }

        //checks to ensure there are no repeated characters
        int counter = 0;

        //lopps through every character in the key
        for (int i = 0; i < 26 ; i++)
        {
            //loops through every character in the key
            for (int j = 0; j < strlen(argv[1]); j++)
            {
                //if the two characters are equal, counter goes up by one (each character should only appear once so the counter should be 26)
                if (argv[1][i] == argv[1][j])
                {
                    counter ++;
                }
            }
        }

        //checks the value of counter, value should not be greater than 26
        if (counter > 26)
        {
            printf(" Key cannot have repeated characters.");
            return 1;
        }

    }

    //prompts the user for text input
    string plaintext = get_string("Plaintext: ");

    //returns ciphertext
    printf("ciphertext: ");

    //loops thorugh every character in the inputted text
    for (int i = 0; i < strlen(plaintext); i++)
    {
        //loops throug every value in the key
        for (int j = 0; j < 26; j++)
        {
            //if the inputted text character is lowercase...
            if (plaintext[i] == 'a' + j)
            {
                //if the corresponding key value is also lowercase...
                if (argv[1][j] >= 'a' && argv[1][j] <= 'z')
                {
                    //return the key value...
                    printf("%c", argv[1][j]);
                }
                else
                {
                    //if not, covert to the uppercase value then print
                    char let1 = argv[1][j] + 32;
                    printf("%c", let1);
                }
            }

            //if the inputted text character is uppercase...
            else if (plaintext[i] == 'A' + j)
            {
                //if the corresponding key value is also uppercase...
                if (argv[1][j] >= 'A' && argv[1][j] <= 'Z')
                {
                    //return the key value...
                    printf("%c", argv[1][j]);
                }
                else
                {
                    //if not, convert to the lowercase value then print
                    char let2 = argv[1][j] - 32;
                    printf("%c", let2);

                }
            }
        }

        //if the character inputted is not an alphabetic character...
        if (isalpha(plaintext[i]) == 0)
        {
            //print as is
            printf("%c", plaintext[i]);
        }

    }

    //return new line to end the text
    printf("\n");
}

