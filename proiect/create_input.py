import random

f = open("input", "w")
size = int(input('Write size: '))
s = str(size) + '\n'
list_of_numbers = list(range(1, size + 2))
for _ in range(1, size + 1):
    random.shuffle(list_of_numbers)
    for j in list_of_numbers:
        s += str(j) + ' '

    s += '\n'

f.write(s)