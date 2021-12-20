with open("input.txt") as file:
    nums = [int(x) for x in file]

increases = 0

i = 0
j = 1

while i < len(nums) - len(nums) % 3:
    sum1 = sum(nums[i:i + 3])
    sum2 = sum(nums[j:j + 3])

    if sum2 > sum1:
        increases += 1

    i +=1
    j +=1

print("Answer:", increases)