from sys import stdin

def part1Nice(string):
    vowelCount = 0
    doubleChar = False
    badWord = False

    for i in range(len(string)):
        if string[i] in 'aeiou':
            vowelCount += 1
        if i < len(string) - 1:
            if string[i] == string[i + 1]:
                doubleChar = True
            if string[i:i+2] in ['ab', 'cd', 'pq', 'xy']:
                badWord = True

    return vowelCount >= 3 and doubleChar and not badWord

def part1(strings):
    niceCount = 0

    for string in strings:
        if part1Nice(string):
            niceCount += 1

    print('Part 1:', niceCount)

def part2Nice(string):
    doubleRepeat = False
    charRepeat = False

    for i in range(len(string)):
        if i < len(string) - 2 and string[i] == string[i + 2]:
            charRepeat = True
        if i < len(string) - 4 and string.find(string[i:i+2], i + 2) != -1:
            doubleRepeat = True

    return doubleRepeat and charRepeat

def part2(strings):
    niceCount = 0

    for string in strings:
        if part2Nice(string):
            niceCount += 1

    print('Part 2:', niceCount)

def main():
    inputLines = []

    for line in stdin:
        inputLines.append(line)

    part1(inputLines)
    part2(inputLines)

main()