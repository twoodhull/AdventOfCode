from sys import stdin

def main():
  sum = 0

  for line in stdin:
    val = int(line)

    while True:
      val = (val // 3) - 2

      if val > 0:
        sum += val
      else:
        break

  print(sum)

main()
