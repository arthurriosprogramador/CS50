def main():
    text = input("Text: ").lower().strip()
    index = calculate_liau_index(text)

    if index > 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


def calculate_liau_index(text):
    words = get_words_quantity(text)
    letters = get_letters_quantity(text)
    sentences = get_sentences_quantity(text)

    l = (letters / words) * 100
    s = (sentences / words) * 100
    index = round((0.0588 * l) - (0.296 * s) - 15.8)
    return index


def get_words_quantity(text):
    words = 0
    for i, char in enumerate(text):
        if char == " " or is_final_punct(char):
            if i > 0 and not is_final_punct(text[i-1]):
                words += 1
    return words


def get_letters_quantity(text):
    letters = 0
    for char in text:
        if char != " " and not is_final_punct(char) and char.isalpha():
            letters += 1
    return letters


def get_sentences_quantity(text):
    sentences = 0
    for char in text:
        if is_final_punct(char):
            sentences += 1
    return sentences


def is_final_punct(char):
    return char in ["!", "?", "."]


main()
