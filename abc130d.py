
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    K = int(buflist[1])
    buf = input()
    buflist = buf.split()
    a = list(map(int, buflist))
    left_pointer = 0
    right_pointer = 0
    combination = 0
    part_sum = 0
    while right_pointer < N:
        part_sum += a[right_pointer]
        while part_sum >= K:
            combination += N - right_pointer
            part_sum -= a[left_pointer]
            left_pointer += 1
        right_pointer += 1
    print(combination)


if __name__ == '__main__':
    main()
