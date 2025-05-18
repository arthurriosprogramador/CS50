#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Prototypes
int calculate_coleman_liau_index(string input);
void get_text_input_to_calculate(void);

int main(void)
{
    get_text_input_to_calculate();
}

// function to get a user input, call the calculate function
// and return the grade to the user
void get_text_input_to_calculate(void)
{
    string input = get_string("Text: ");
    int index = calculate_coleman_liau_index(input);

    if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

// function that make the Coleman-Liau index calculus
int calculate_coleman_liau_index(string input)
{
    int input_size = strlen(input);
    int sentences = 0;
    int letters = 0;
    int words = 0;
    for (int i = 0; i < input_size; i++)
    {
        if (input[i] == '!' || input[i] == '.' || input[i] == '?')
        {
            sentences++;
        }

        if (!isblank(input[i]) && isalpha(input[i]))
        {
            letters++;
        }

        if (isblank(input[i]))
        {
            words++;
        }

        if (i == input_size - 1)
        {
            words++;
        }
    }

    float l = (letters / (float) words) * 100.0;
    float s = (sentences / (float) words) * 100.0;
    float index = 0.0588 * l - 0.296 * s - 15.8;

    return round(index);
}
