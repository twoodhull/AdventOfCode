def main():
	numStrings = input().split(',')
	nums = []

	for i in numStrings:
		nums.append(int(i))

	nums[1] = 12
	nums[2] = 2

	i = 0
	while True:
		opcode = nums[i]

		if opcode == 1:
			nums[nums[i + 3]] = nums[nums[i + 1]] + nums[nums[i + 2]]
		elif opcode == 2:
			nums[nums[i + 3]] = nums[nums[i + 1]] * nums[nums[i + 2]]
		elif opcode == 99:
			break

		i += 4

	print(nums[0])

main()