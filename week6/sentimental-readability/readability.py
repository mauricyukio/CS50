# Computes the approximate grade level needed to comprehend a text

from cs50 import get_string

# Prompts user for text to be analysed
text = get_string("Text: ")

# Counts the number of letters in the text provided
letters = 0
for i in range(0, len(text)):
    if text[i].isalpha():
        letters += 1

# Counts the number of spaces in the text provided, then calculates number of words
spaces = 0
for i in range(0, len(text)):
    if text[i] == " ":
        spaces += 1
words = spaces + 1

# Counts the number of sentences in the text provided
sentences = 0
for i in range(0, len(text)):
    if text[i] in [".", "!", "?"]:
        sentences += 1

# Calculates the Coleman-Liau index
L = letters / words * 100
S = sentences / words * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

# Prints out the grade that corresponds to the Coleman-Liau index
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print("Grade " + str(index))