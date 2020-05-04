mod = int(10 ** 9) + 7

def main():
    n = int(input())
    c = list(map(int, input().split()))

    c.sort()
    pow2 = [None for _ in range(2*n+1)]
    pow2[0] = 1
    for i in range(1, 2*n+1):
        pow2[i] = (pow2[i-1] * 2) % mod

    total_cost = [None for _ in range(n)]
    for i in range(n):
        k = n - i - 1
        p2k2 = (pow2[k-1] * (k + 2)) % mod
        if k-1 < 0:
            p2k2 = 1
        total_cost[i] = (((p2k2 * c[i]) % mod) * pow2[2*i] * pow2[k+1]) % mod

    ans = sum(total_cost) % mod

    print(ans)

if __name__ == '__main__':
    main()
