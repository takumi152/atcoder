
mod = 1000000007

def main():
    n, k = map(int, input().split())
    a = list(map(int, input().split()))

    ans = 0
    for i in range(n):
        for j in range(n):
            if a[i] > a[j]:
                inv = 0
                if (i < j):
                    inv = (((k + 1) * k) // 2) % mod
                else:
                    inv = ((k * (k - 1)) // 2) % mod
                ans = (ans + inv) % mod

    print(ans)


if __name__ == '__main__':
    main()
