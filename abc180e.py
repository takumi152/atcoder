def main():
    n = int(input())
    x = [0 for _ in range(n)]
    y = [0 for _ in range(n)]
    z = [0 for _ in range(n)]
    for i in range(n):
        x[i], y[i], z[i] = map(int, input().split())

    dp = [[10 ** 18 for _ in range(n)] for _ in range(2 ** n)]
    dp[1][0] = 0
    for i in range(1, 2 ** n):
        for k in range(n):
            if (i & (1 << k) > 0):
                for j in range(1, n):
                    if i & (1 << j) == 0:
                        dp[i|(1<<j)][j] = min(dp[i|(1<<j)][j], dp[i][k] + abs(x[k] - x[j]) + abs(y[k] - y[j]) + max(0, z[k] - z[j]))

    ans = 10 ** 18
    for i in range(1, n):
        ans = min(ans, dp[2**n-1][i] + abs(x[i] - x[0]) + abs(y[i] - y[0]) + max(0, z[i] - z[0]))

    print(ans)

if __name__ == '__main__':
    main()
