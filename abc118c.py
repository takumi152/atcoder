
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    remaining = list(sorted(set(A)))
    while len(remaining) > 1:
        smallest = remaining[0]
        for i in range(1, len(remaining)):
            remaining[i] %= smallest
        new_remaining = []
        for i in remaining:
            if i > 0:
                new_remaining.append(i)
        remaining = list(sorted(set(new_remaining)))
    print(remaining[0])

if __name__ == '__main__':
    main()
