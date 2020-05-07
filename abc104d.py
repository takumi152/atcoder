
mod = 10 ** 9 + 7

def main():
    s = input()

    n = len(s)

    dp = [[0 for _ in range(4)] for _ in range(n+1)]
    dp[0][0] = 1
    for i in range(n):
        if s[i] in ('A', '?'):
            dp[i+1][0] = (dp[i+1][0] + dp[i][0]) % mod
            dp[i+1][1] = (dp[i+1][1] + dp[i][1]) % mod
            dp[i+1][2] = (dp[i+1][2] + dp[i][2]) % mod
            dp[i+1][3] = (dp[i+1][3] + dp[i][3]) % mod
            dp[i+1][1] = (dp[i+1][1] + dp[i][0]) % mod
        if s[i] in ('B', '?'):
            dp[i+1][0] = (dp[i+1][0] + dp[i][0]) % mod
            dp[i+1][1] = (dp[i+1][1] + dp[i][1]) % mod
            dp[i+1][2] = (dp[i+1][2] + dp[i][2]) % mod
            dp[i+1][3] = (dp[i+1][3] + dp[i][3]) % mod
            dp[i+1][2] = (dp[i+1][2] + dp[i][1]) % mod
        if s[i] in ('C', '?'):
            dp[i+1][0] = (dp[i+1][0] + dp[i][0]) % mod
            dp[i+1][1] = (dp[i+1][1] + dp[i][1]) % mod
            dp[i+1][2] = (dp[i+1][2] + dp[i][2]) % mod
            dp[i+1][3] = (dp[i+1][3] + dp[i][3]) % mod
            dp[i+1][3] = (dp[i+1][3] + dp[i][2]) % mod

    print(dp[n][3])

if __name__ == '__main__':
    main()
