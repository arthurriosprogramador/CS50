MIN_DIGITS = 13
MAX_DIGITS = 16
FIFTEEN_DIGITS = 15
VISA_INITIAL_NUMBER = 4
AMEX_INITIAL_NUMBER_34 = 34
AMEX_INITIAL_NUMBER_37 = 37
MASTERCARD_INITIAL_NUMBER = range(51, 56)


def main():
    card_number = ""
    card_number = input("Number: ").strip()
    if "-" in card_number:
        print("INVALID")
        return 1

    card_number_size = len(card_number)
    is_valid = is_valid_luhns(card_number, card_number_size)
    verify_card_type(card_number, card_number_size, is_valid)


def verify_card_type(card_number, card_number_size, is_valid):
    first_digit = int(card_number[0])
    first_two_digits = int(card_number[:2])
    if not is_valid:
        print("INVALID")
    else:
        if (first_two_digits == AMEX_INITIAL_NUMBER_34 or first_two_digits == AMEX_INITIAL_NUMBER_37) and card_number_size == FIFTEEN_DIGITS:
            print("AMEX")
        elif first_two_digits in MASTERCARD_INITIAL_NUMBER and card_number_size == MAX_DIGITS:
            print("MASTERCARD")
        elif first_digit == VISA_INITIAL_NUMBER and (card_number_size == MIN_DIGITS or card_number_size == MAX_DIGITS):
            print("VISA")
        else:
            print("INVALID")


def is_valid_luhns(card_number, card_number_size):
    reversed_number = card_number[::-1]
    doubled_numbers_sum = 0
    not_doubled_numbers_sum = 0
    for i in range(card_number_size):
        if i % 2 == 1:
            doubled = int(reversed_number[i]) * 2
            if doubled > 9:
                doubled_numbers_sum += int((doubled / 10))
                doubled_numbers_sum += int((doubled % 10))
            else:
                doubled_numbers_sum += doubled
        else:
            not_doubled_numbers_sum += int(reversed_number[i])

    final_sum = doubled_numbers_sum + not_doubled_numbers_sum
    last_number = final_sum % 10
    return last_number == 0


main()
