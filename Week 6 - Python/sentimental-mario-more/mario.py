def main():
    height = 0

    while True:
        try:
            height = int(input("Height: "))
            if height <= 0 or height > 8:
                raise ValueError()
            break
        except ValueError:
            print("That's not a valid input. Please, type an integer greater than 0 and smaller or equal to 8!")

    for i in range(1, height+1):
        build_left_side(i, height-i)
        print("  ", end="")
        build_right_side(i, height-i)

def build_left_side(height, spaces):
    print(" " * spaces, end="")
    print("#" * height, end="")

def build_right_side(height, spaces):
    print("#" * height, end="")
    print(" " * spaces)

main()
