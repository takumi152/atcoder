def main():
    n = int(input())
    a = list(map(int, input().split()))

    if n == 1:
        print(a[0])
        return

    ans = 2 ** 31
    for i in range(2 ** (n - 1)):
        res = 0
        now = 0
        for j in range(n-1):
            now = now | a[j]
            if i & (1 << j):
                res = res ^ now
                now = 0
        now = now | a[n-1]
        res = res ^ now
        if res < ans:
            ans = res

    print(ans)

if __name__ == '__main__':
    main()
