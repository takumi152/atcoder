
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    B = list(map(int, buflist))
    if sum(A) < sum(B):
        print(-1) # impossible
        return
    diff = []
    for i in range(N):
        diff.append(A[i] - B[i])
    diff = list(sorted(diff))
    insufficient_value = 0
    insufficient_count = 0
    for i in range(N):
        if diff[i] < 0:
            insufficient_value += -diff[i]
            insufficient_count += 1
    deduction_count = 0
    pointer = -1
    while insufficient_value > 0:
        insufficient_value -= diff[pointer]
        deduction_count += 1
        pointer -= 1
    print(insufficient_count + deduction_count)


if __name__ == '__main__':
    main()
