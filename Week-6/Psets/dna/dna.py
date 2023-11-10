import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable
    # list to store all the person's dna data
    data = []

    # opens the csv file giveen in cli argument
    with open(sys.argv[1], "r") as f:
        # reads the file
        reader = csv.reader(f)
        # this gets the first row / header of the csv file
        header = next(reader)

        # goes through all the data in the file and adds it into a dict
        for i in reader:
            person = {}
            for j in range(len(header)):
                person[header[j]] = i[j]
                if j != 0:
                    person[header[j]] = int(i[j])
                else:
                    person[header[j]] = i[j]
            data.append(person)

    DNAs = header
    # pops name from the list
    DNAs.remove("name")

    # TODO: Read DNA sequence file into a variable
    suspect = {}
    for i in DNAs:
        suspect[i] = 0
    with open(sys.argv[2], "r") as f:
        suspect_dna = f.read()

    # TODO: Find longest match of each STR in DNA sequence
    for i in DNAs:
        suspect[i] = longest_match(suspect_dna, i)

    # TODO: Check database for matching profiles
    found = False
    for person in data:
        found = False
        for dna in DNAs:
            if person[dna] == suspect[dna]:
                found = True
                name = person['name']
            else:
                found = False
                break
        if found:
            break
    if found:
        print(f"{name}")
    else:
        print("No match")
    return


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
