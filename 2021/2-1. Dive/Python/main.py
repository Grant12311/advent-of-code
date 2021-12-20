infile = open("input.txt")

pos = 0
depth = 0

for line in infile:
    spacePos = line.find(' ')

    command = line[:spacePos]
    ammount = int(line[spacePos:])

    if command == "forward":
        pos += ammount
    elif command == "up":
        depth -= ammount
    else:
        depth += ammount

print("Answer:", pos * depth)

infile.close()