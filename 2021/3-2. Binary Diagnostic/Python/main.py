def mostSignificantBit(a_data, a_index):
    ones = 0
    zeros = 0

    for i in range(len(a_data)):
        if a_data[i][a_index] == '1':
            ones += 1
        else:
            zeros += 1

    return '1' if ones >= zeros else '0'

def leastSignificantBit(a_data, a_index):
    ones = 0
    zeros = 0

    for i in range(len(a_data)):
        if a_data[i][a_index] == '1':
            ones += 1
        else:
            zeros += 1

    return '1' if ones < zeros else '0'

with open("input.txt") as file:
    data = file.readlines()

dataCopy = [x for x in data]

for i in range(12):
    if len(dataCopy) == 1:
        break

    sig = mostSignificantBit(dataCopy, i)

    dataCopy = [x for x in dataCopy if x[i] == sig]

oxygen = int(dataCopy[0], 2)

dataCopy = [x for x in data]

for i in range(12):
    if len(dataCopy) < 2:
        break

    least = leastSignificantBit(dataCopy, i)

    dataCopy = [x for x in dataCopy if x[i] == least]

co2 = int(dataCopy[0], 2)

print("Answer:", oxygen * co2)