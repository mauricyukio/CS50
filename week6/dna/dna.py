import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        quit()

    # Read database file into a variable
    dnaDict = []
    subsequences = []
    with open(sys.argv[1], "r") as file:
        reader = csv.reader(file)
        subsequences = next(reader)
        dictReader = csv.DictReader(file, fieldnames=subsequences)
        for row in dictReader:
            dnaDict.append(row)

    # Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        sequence = file.read()

    # Find longest match of each STR in DNA sequence
    del subsequences[0]

    longest_runs = []
    for i in range(len(subsequences)):
        longest_runs.append(longest_match(sequence, subsequences[i]))

    # Check database for matching profiles
    for i in range(len(dnaDict)):
        match = 0
        for j in range(len(subsequences)):
            if int(dnaDict[i][subsequences[j]]) != longest_runs[j]:
                break
            else:
                match += 1
        if match == len(longest_runs):
            print(dnaDict[i]['name'])
            quit()
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
