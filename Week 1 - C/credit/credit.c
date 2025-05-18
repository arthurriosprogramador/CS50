#include <cs50.h>
#include <stdio.h>

const int MAX_DIGITS = 16;
const int FIFTEEN_DIGITS = 15;
const int MIN_DIGITS = 13;
const int VISA_INITIAL_NUMBER = 4;
const int AMEX_INITIAL_NUMBER_34 = 34;
const int AMEX_INITIAL_NUMBER_37 = 37;
const int MASTERCARD_INITIAL_NUMBER_51 = 51;
const int MASTERCARD_INITIAL_NUMBER_55 = 55;

int count_digits(long input, int digits[MAX_DIGITS]);
long get_power_ten(int exponent);
void verify_card_type(long card_number, int card_digits, bool is_number_valid, int digits_size);
bool is_valid_luhn(long card_number, int card_digits[MAX_DIGITS], int card_digits_size);

int main(void)
{
    long card_number;
    int digits[MAX_DIGITS];
    int digits_size;

    card_number = get_long("Card number: ");
    digits_size = count_digits(card_number, digits);

    bool is_valid = is_valid_luhn(card_number, digits, digits_size);
    verify_card_type(card_number, digits_size, is_valid, digits_size);
}

int count_digits(long input, int digits[MAX_DIGITS])
{
    int count = 0;
    while (input > 0)
    {
        digits[count] = input % 10;
        input /= 10;
        count++;
    }

    return count;
}

long get_power_ten(int exponent)
{
    long power = 1;
    for (int i = 0; i < exponent; i++)
    {
        power *= 10;
    }
    return power;
}

void verify_card_type(long card_number, int card_digits, bool is_number_valid, int digits_size)
{
    long first_digit = card_number / get_power_ten(card_digits - 1);
    long first_two_digits = card_number / get_power_ten(card_digits - 2);

    if (digits_size >= MIN_DIGITS)
    {
        if (first_digit == VISA_INITIAL_NUMBER && is_number_valid &&
            (digits_size == MIN_DIGITS || digits_size == MAX_DIGITS))
        {
            printf("VISA\n");
        }
        else if ((first_two_digits == AMEX_INITIAL_NUMBER_34 ||
                  first_two_digits == AMEX_INITIAL_NUMBER_37) &&
                 is_number_valid && digits_size == FIFTEEN_DIGITS)
        {
            printf("AMEX\n");
        }
        else if ((first_two_digits >= MASTERCARD_INITIAL_NUMBER_51 &&
                  first_two_digits <= MASTERCARD_INITIAL_NUMBER_55) &&
                 is_number_valid && digits_size == MAX_DIGITS)
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else if (digits_size > MAX_DIGITS)
    {
        printf("INVALID\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

bool is_valid_luhn(long card_number, int card_digits[MAX_DIGITS], int card_digits_size)
{
    int doubled_numbers_sum = 0;
    int not_doubled_numbers_sum = 0;
    for (int i = 0; i < card_digits_size; i++)
    {
        if (i % 2 == 1)
        {
            int doubled = card_digits[i] * 2;
            if (doubled > 9)
            {
                doubled_numbers_sum += (doubled / 10);
                doubled_numbers_sum += (doubled % 10);
            }
            else
            {
                doubled_numbers_sum += doubled;
            }
        }
        else
        {
            not_doubled_numbers_sum += card_digits[i];
        }
    }

    int final_sum = doubled_numbers_sum + not_doubled_numbers_sum;
    if (final_sum % 10 == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
