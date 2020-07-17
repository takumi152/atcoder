def main():
    n = int(input())
    a = list(map(int, input().split()))

    a.sort()

    ans = 0
    sieve = [False for _ in range(1000001)]
    for i in range(n):
        if sieve[a[i]]:
            continue
        for j in range(a[i], 1000001, a[i]):
            sieve[j] = True
        if i > 0:
            if a[i] == a[i-1]:
                continue
        if i < n-1:
            if a[i] == a[i+1]:
                continue
        ans += 1

    print(ans)


if __name__ == '__main__':
    main()
