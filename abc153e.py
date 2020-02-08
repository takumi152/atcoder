def main():
    h, n = map(int, input().split())
    ab = []
    for i in range(n):
        ab.append(tuple(map(int, input().split())))

    dp = []
    for i in range(h+1):
        dp.append(10**18)
    dp[h] = 0
    for i in range(h, 0, -1):
        for j in range(n):
            dp[max(0, i-ab[j][0])] = min(dp[max(0, i - ab[j][0])], dp[i] + ab[j][1])

    print(dp[0])


if __name__ == '__main__':
    main()
