import bisect

def main():
    buf = input()
    N = int(buf)
    withdrawal = []
    withdrawal += [int(6 ** x) for x in range(10)]
    withdrawal += [int(9 ** x) for x in range(10)]
    withdrawal = list(sorted(withdrawal))
    stack = [[N, 0]]
    best = 999
    while stack:
        current = stack.pop()
        if current[1] > best:
            continue
        if current[0] == 0:
            if current[1] < best:
                best = current[1]
            continue
        point = bisect.bisect_right(withdrawal, current[0])
        stack.append([current[0]-withdrawal[point-1], current[1]+1])
        stack.append([current[0]-withdrawal[point-2], current[1]+1])
    print(best)

if __name__ == '__main__':
    main()
