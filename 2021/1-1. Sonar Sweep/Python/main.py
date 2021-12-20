infile = open("input.txt")

prev: int = int(infile.readline())
current: int = 0

increases: int = 0

while True:
    try:
        current = int(infile.readline())
    except:
        break

    if current > prev:
        increases += 1

    prev = current

print("Answer:", increases)

infile.close()