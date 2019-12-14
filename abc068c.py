
def main():
    buf = input()
    buflist = buf.split()
    n = int(buflist[0])
    m = int(buflist[1])
    ab = []
    for i in range(m):
        buf = input()
        buflist = buf.split()
        ab.append(tuple(map(int, buflist)))

    ab.sort()
    dp = []
    for i in range(n+1):
        dp.append([False, False, False])
    dp[1][0] = True
    for i in range(m):
        if dp[ab[i][0]][0]:
            dp[ab[i][1]][1] = True
        if dp[ab[i][0]][1]:
            dp[ab[i][1]][2] = True
    if dp[n][2]:
        print('POSSIBLE')
    else:
        print('IMPOSSIBLE')

if __name__ == '__main__':
    main()
