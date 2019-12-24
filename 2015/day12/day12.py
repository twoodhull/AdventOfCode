import re
import json

def parseJson(jsonObj, isRed = False):
    if isinstance(jsonObj, dict):
        for _, value in jsonObj.items():
            if value == "red":
                isRed = True
                break

        sum1 = 0
        sum2 = 0
        for _, value in jsonObj.items():
            sumPair = parseJson(value, isRed)
            sum1 += sumPair[0]
            sum2 += sumPair[1]

        return (sum1, sum2)
    elif isinstance(jsonObj, list):
        sum1 = 0
        sum2 = 0

        for child in jsonObj:
            sumPair = parseJson(child, isRed)
            sum1 += sumPair[0]
            sum2 += sumPair[1]

        return (sum1, sum2)
    elif isinstance(jsonObj, int):
        return (jsonObj, 0 if isRed else jsonObj)
    else:
        return (0, 0)

def main():
    jsonStr = input()

    jsonObj = json.loads(jsonStr)
    sumPair = parseJson(jsonObj)

    part1Sum = sumPair[0]
    part2Sum = sumPair[1]

    print('Part 1:', part1Sum)
    print('Part 2:', part2Sum)

main()
