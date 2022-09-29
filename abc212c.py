def main():
    n, m = map(int, input().split())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    a.sort()
    b.sort()

    min_diff = 10 ** 18
    ap = 0
    bp = 0
    while ap < n and bp < m:
        if abs(a[ap] - b[bp]) < min_diff:
            min_diff = abs(a[ap] - b[bp])
        if a[ap] < b[bp]:
            ap += 1
        else:
            bp += 1

    print(min_diff)

if __name__ == '__main__':
    main()
