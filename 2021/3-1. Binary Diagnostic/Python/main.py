with open("input.txt") as file:
    data = file.readlines()

gammaRate = ['0'] * 12

for i in range(len(gammaRate)):
    ones = 0
    zeroes = 0

    for num in data:
        if num[i] == '1':
            ones += 1
        else:
            zeroes += 1

    gammaRate[i] = '1' if ones > zeroes else '0'

epsilonRate = ['0'] * 12

for i in range(len(gammaRate)):
    ones = 0
    zeroes = 0

    for num in data:
        if num[i] == '1':
            ones += 1
        else:
            zeroes += 1

    epsilonRate[i] = '1' if ones < zeroes else '0'

print("Answer:", int("".join(gammaRate), 2) * int("".join(epsilonRate), 2))