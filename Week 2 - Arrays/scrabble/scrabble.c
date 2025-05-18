#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// const score array
const char FIRST_LETTER = 'A';
const int scores[26] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                        1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
// prototypes
int calculate_score(string input);
void define_result(string player_1_input, string player_2_input);
void get_inputs_to_calculate(void);

int main(void)
{
    get_inputs_to_calculate();
}

void get_inputs_to_calculate(void)
{
    string player_1_input = get_string("Player 1: ");
    string player_2_input = get_string("Player 2: ");

    define_result(player_1_input, player_2_input);
}

int calculate_score(string input)
{
    int input_size = strlen(input);
    int score = 0;
    for (int i = 0; i < input_size; i++)
    {
        char letter = toupper(input[i]);
        score += scores[letter - FIRST_LETTER];
    }

    return score;
}

void define_result(string player_1_input, string player_2_input)
{
    int score_player_1 = calculate_score(player_1_input);
    int score_player_2 = calculate_score(player_2_input);

    if (score_player_1 > score_player_2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score_player_2 > score_player_1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}
