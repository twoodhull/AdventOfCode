import hashlib

def find(prefix):
    i = 0
    while True:
        i += 1

        string = 'iwrupvqb' + str(i)
        if (hashlib.md5(string.encode()).hexdigest()[:len(prefix)] == prefix):
            return i

def part1():
    print('Part 1:', find('00000'))

def part2():
    print('Part 2:', find('000000'))

def main():
    part1()
    part2()

main()