import csv
from sys import argv


def main():
    # Check arguments
    if len(argv) < 3:
        print("Usage: python dna.py databases sequences")

    # Read the database
    database_rows = []
    with open(argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            database_rows.append(row)

    # Read the sequences
    sequence = ""
    with open(argv[2]) as file:
        sequence = file.read()

    # Get the sequences in the database to find the longest match
    header = list(database_rows[0].keys())
    sequences_to_analyze = header[1:len(header)+1]

    analyzed_sequences = []
    for sequence_to_analyze in sequences_to_analyze:
        analyzed_sequences.append(longest_match(sequence, sequence_to_analyze))

    # Check database for matching profiles
    name = ""
    found = False
    for database_row in database_rows:
        if found == True:
            break
        for i, key in enumerate(sequences_to_analyze):
            if int(analyzed_sequences[i]) != int(database_row[key]):
                name = ""
                break
            else:
                if i == (len(sequences_to_analyze) - 1):
                    found = True
                    name = database_row["name"]

    if name == "":
        print("No match")
    else:
        print(name)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
