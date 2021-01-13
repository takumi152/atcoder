def main():
    n = int(input())
    ab = [None for _ in range(n)]
    for i in range(n):
        ab[i] = tuple(map(int, input().split()))

    total = 0
    for i in range(n):
        total += ab[i][0]

    ab.sort(key = lambda x: x[0] * 2 + x[1], reverse = True)

    gain = 0
    ans = 0
    for i in  range(n):
        gain += ab[i][0] * 2 + ab[i][1]
        ans += 1
        if gain > total:
            break

    print(ans)


if __name__ == '__main__':
    main()
