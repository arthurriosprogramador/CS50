QUARTER = 25
DIME = 10
NICKEL = 5
PENNY = 1


def main():
    change = 0.0
    coins = 0

    while True:
        try:
            change = float(input("Change: ")) * 100
            if change < 0:
                raise ValueError()
            break
        except ValueError:
            print("That's not a valid input!")

    while change > 0.0:
        coin = verify_change_and_get_coin(change)
        coin_quantity = calculate_coin_quantity(change, coin)
        coins += coin_quantity
        change -= int(coin_quantity) * coin

    print(coins)


def verify_change_and_get_coin(change):
    coin = (
        DIME if change < QUARTER and change >= DIME else
        NICKEL if change < DIME and change >= NICKEL else
        PENNY if change < NICKEL and change >= PENNY else
        QUARTER
    )
    return coin


def calculate_coin_quantity(change, coin):
    if change >= coin:
        coin_quantity = change / coin
        return int(coin_quantity)
    else:
        return 0


main()
