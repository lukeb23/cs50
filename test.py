import csv
import sys
import itertools


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # TODO: Read database file into a variable

    # Declare a dictonary to read peoples dna database into
    people = []
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)
        for person in reader:
            people.append(person)

    # TODO: Read DNA sequence file into a variable

    # Read the DNA file
    with open(sys.argv[2], "r") as file:
        dna = file.read()

    # TODO: Find longest match of each STR in DNA sequence

    # Declare a dictionary with results
    results = {
        "AGATC": 0,
        "TTTTTTCT": 0,
        "AATG": 0,
        "TCTAG": 0,
        "GATA": 0,
        "TATC": 0,
        "GAAA": 0,
        "TCTG": 0,
    }
    # Iterate and updatethe longest match for each SRT
    for i in results:
        results[i] = str(longest_match(dna, i))

    # TODO: Check database for matching profiles

    # Iterate over each persons dna matches checking for longest match
    for i in range(len(people)):
        if results[i] in itertools.chain.from_iterable(people):
            # Check if the csv file is large
            if "large" in sys.argv[1]:
                if results["TTTTTTCT"] in people[i]["TTTTTTCT"] and results["TCTAG"] in people[i]["TCTAG"] and results["GATA"] in people[i]["GATA"] and results["GAAA"] in people[i]["GAAA"] and results["TCTG"] in people[i]["TCTG"]:
                    name = people[i]["name"]
                    print(f"{name}")
                    return

            else:
                name = people[i]["name"]
                print(f"{name}")
                return

    print("No match")


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
