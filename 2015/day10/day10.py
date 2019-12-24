
def digCount(str, digit, index):
    count = 0

    while index < len(str) and str[index] == digit:
        count += 1
        index += 1

    return count

def main():
    val = input()

    part1 = 0
    part2 = 0

    for i in range(50):
        newVal = ''

        index = 0
        while index < len(val):
            count = digCount(val, val[index], index)
            newVal += str(count) + val[index]
            index += count

        val = newVal

        if i == 39:
            part1 = len(val)
        elif i == 49:
            part2 = len(val)

    print(part1)
    print(part2)

main()