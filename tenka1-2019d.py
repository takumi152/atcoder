# WIP

MOD = 998244353

def main():
    buf = input()
    N = int(buf)
    a = []
    total = 0
    for i in range(N):
        buf = input()
        num = int(buf)
        a.append(num)
        total += num
    dp = []
    # i 合計個数
    # j A+Bに入れた個数
    # k A+B
    # v あり得る組み合わせ数(MOD 998244353)
    for i in range(N+1):
        dp.append([])
        for j in range(N+1):
            dp[i].append([])
            for k in range(total+1):
                dp[i][j].append(0)
    dp[0][0][0] = 1
    for i in range(N):
        for j in range(N+1):
            for k in range(total+1):
                dp[i+1][j][k] += dp[i][j][k]
        for j in range(N):
            for k in range(total+1 - a[i]):
                dp[i+1][j+1][k+a[i]] += dp[i][j][k]
                dp[i+1][j+1][k+a[i]] %= MOD
    for i in range(N+1):
        print(dp[i])
    count = 0
    for j in range(2, N):
        for k in range(total//2+1, total+1):
            count += dp[N][j][k]
            count %= MOD
    print(count)


if __name__ == '__main__':
    main()
