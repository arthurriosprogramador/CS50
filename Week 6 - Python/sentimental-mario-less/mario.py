height = 0

while True:
    try:
        height = int(input("Height: "))
        break
    except ValueError:
        print("That's not a valid input. Please, type an integer!")


for i in range(1, height+1):
    print(" " * (height - i), end="")
    print("#" * i)
