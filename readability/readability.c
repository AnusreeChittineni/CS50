#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//declares function so program recognizes it
void What_grade(string word);

int main(void)
{
    //prompts user for text input
    string text = get_string("Text: ");

    //calls custom function with argument text
    What_grade(text);
}

//function used to calcualte readability grade
void What_grade(string words)
{
    //assignes 0 as initial variable values
    int number_letters = 0;
    int number_words = 0;
    int number_sentences = 0;

    //accounts for the first word in the text if there is not a space as the first character
    if (isgraph(words[0]) != 0)
    {
        number_words += 1;
    }

    //loops through each character in the text to determine the number of letters,words, and sentences
    for (int i = 0; i < strlen(words); i++)
    {
        if (isupper(words[i]) != 0 || islower(words[i]) != 0)
        {
            number_letters += 1;
        }
        else if (((isspace(words[i]) != 0 && isgraph(words[i + 1]) != 0)))
        {
            number_words += 1;
        }

        else if ((words[i] == '.') || (words[i] == '!') || (words[i] == '?'))
        {
            number_sentences += 1;
        }
    }

    //formula used to calculate grade level of readability
    float grade = (0.0588 * (((float)number_letters / number_words) * 100) - 0.296 * (((float)number_sentences / number_words) * 100) -
                   15.8);

    //determines what to output based on the grade level of readability after rounding
    if (round(grade) >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (round(grade) < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 1 && grade < 16)
    {
        printf("Grade %i\n", (int) round(grade));
    }
}