#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// constants
const char FIRST_LETTER = 'A';

// prototypes
void get_input_and_encrypt(string key, int key_size);

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        string key = argv[1];
        int key_size = strlen(key);

        for (int i = 0; i < key_size; i++)
        {
            if (!isalpha(key[i]))
            {
                printf("The key must contain only letters.\n");
                return 1;
            }

            // verify if there are any duplicate letter
            for (int j = i + 1; j < key_size; j++)
            {
                if (toupper(key[i]) == toupper(key[j]))
                {
                    printf("The key must not have duplicate letters.\n");
                    return 1;
                }
            }
        }

        if (key_size != 26)
        {
            printf("The key must contain 26 characters\n");
            return 1;
        }
        else
        {
            get_input_and_encrypt(key, key_size);
            return 0;
        }
    }
    else if (argc < 2)
    {
        printf("Please, run the program with the key as argument.\n");
        return 1;
    }
    else
    {
        printf("Please, run the program with e one argument.\n");
        return 1;
    }
}

void get_input_and_encrypt(string key, int key_size)
{
    string plain_text = get_string("plaintext: ");
    int plain_text_size = strlen(plain_text);

    char cipher_text[plain_text_size];

    for (int i = 0; i < plain_text_size; i++)
    {
        if (!isalpha(plain_text[i]))
        {
            cipher_text[i] = plain_text[i];
        }
        else
        {
            int position = toupper(plain_text[i]) - FIRST_LETTER;
            cipher_text[i] =
                isupper(plain_text[i]) ? toupper(key[position]) : tolower(key[position]);
        }
    }
    cipher_text[plain_text_size] = '\0';
    printf("ciphertext: %s\n", cipher_text);
}
