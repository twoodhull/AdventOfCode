def runProgram(nums, noun, verb):
	programNums = []
	for i in nums:
		programNums.append(i)

	programNums[1] = noun
	programNums[2] = verb

	i = 0
	while True:
		opcode = programNums[i]

		if opcode == 1:
			programNums[programNums[i + 3]] = programNums[programNums[i + 1]] + programNums[programNums[i + 2]]
		elif opcode == 2:
			programNums[programNums[i + 3]] = programNums[programNums[i + 1]] * programNums[programNums[i + 2]]
		elif opcode == 99:
			break

		i += 4

	return programNums[0]

def main():
	numStrings = input().split(',')
	nums = []

	for i in numStrings:
		nums.append(int(i))

	for i in range(0, 100):
		for j in range(0, 100):
			if runProgram(nums, i, j) == 19690720:
				print(i, j, '--', (100 * i + j))
				break

main()