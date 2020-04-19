mod = int(1e9) + 7

def main():
    n, k = map(int, input().split())

    small = 0
    large = 0
    ans = 0
    for i in range(0, n+1):
        small += i
        large += n-i
        if i+1 >= k:
            ans = (ans + (large - small + 1)) % mod

    print(ans)


if __name__ == '__main__':
    main()
