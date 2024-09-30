# TODO
from cs50 import get_float

# Prompt user for amount of cash wanting to be converted
while True:
    cash = get_float("Amount of cash: ")
    if cash > 0:
        break

# Convert cash to cents
cents = round(cash * 100)

# Intialize change count
change = 0

# Check for each coin type and update change count
while cents >= 25:
    cents -= 25
    change += 1

while cents >= 10:
    cents -= 10
    change += 1

while cents >= 5:
    cents -= 5
    change += 1

while cents >= 1:
    cents -= 5
    change += 1

# Print change
print(f"Change owed {change}")
