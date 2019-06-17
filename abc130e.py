MOD = int(1e9) + 7

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    buf = input()
    buflist = buf.split()
    S = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    T = list(map(int, buflist))
    dp = []
    for i in range(N+1):
        dp.append([])
        for j in range(M+1):
            dp[i].append(1)
    for i in range(N):
        for j in range(M):
            if S[i] == T[j]:
                dp[i+1][j+1] = (dp[i+1][j] + dp[i][j+1]) % MOD
            else:
                dp[i+1][j+1] = (dp[i+1][j] + dp[i][j+1] - dp[i][j]) % MOD
    print(dp[N][M])

if __name__ == '__main__':
    main()
