infile = open("input.txt")

pos = 0
depth = 0
aim = 0

for line in infile:
    command, ammount = line.split()

    ammount = int(ammount)

    if command == "forward":
        pos += ammount
        depth += ammount * aim
    elif command == "up":
        aim -= ammount
    else:
        aim += ammount

print("Answer:", pos * depth)

infile.close()