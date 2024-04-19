def local_align(s, t):
    n = len(s)
    m = len(t)

    dp = [[-1000000007 for _ in range(m+1)] for _ in range(n+1)]
    prev = [[None for _ in range(m+1)] for _ in range(n+1)]
    dp[0][0] = 0
    for i in range(n):
        for j in range(m):
            if s[i] == t[j]:
                if max(dp[i+1][j+1], dp[i][j] + 1) == dp[i][j] + 1:
                    dp[i+1][j+1] = max(dp[i+1][j+1], dp[i][j] + 1)
                    prev[i+1][j+1] = (i, j)
            else:
                if max(dp[i+1][j+1], dp[i][j] - 3) == dp[i][j] - 3:
                    dp[i+1][j+1] = max(dp[i+1][j+1], dp[i][j] - 3)
                    prev[i+1][j+1] = (i, j)
            if max(dp[i+1][j], dp[i][j] - 5) == dp[i][j] - 5:
                dp[i+1][j] = max(dp[i+1][j], dp[i][j] - 5)
                prev[i+1][j] = (i, j)
            if max(dp[i][j+1], dp[i][j] - 5) == dp[i][j] - 5:
                dp[i][j+1] = max(dp[i][j+1], dp[i][j] - 5)
                prev[i][j+1] = (i, j)
        if max(dp[i+1][m], dp[i][m] - 5) == dp[i][m] - 5:
            dp[i+1][m] = max(dp[i+1][m], dp[i][m] - 5)
            prev[i+1][m] = (i, m)
    for j in range(m):
        if max(dp[n][j+1], dp[n][j] - 5) == dp[n][j] - 5:
            dp[n][j+1] = max(dp[n][j+1], dp[n][j] - 5)
            prev[n][j+1] = (n, j)

    u_space = []
    v_space = []
    p = n
    q = m
    while p > 0 or q > 0:
        if prev[p][q][0] == p:
            u_space.append(p)
        if prev[p][q][1] == q:
            v_space.append(q)
        p, q = prev[p][q]

    ans_u = ''
    ans_v = ''
    for i in range(n):
        while u_space:
            if u_space[-1] == i:
                ans_u += '-'
                u_space.pop()
            else:
                break
        ans_u += s[i]
    while u_space:
        ans_u += '-'
        u_space.pop()
    for i in range(m):
        while v_space:
            if v_space[-1] == i:
                ans_v += '-'
                v_space.pop()
            else:
                break
        ans_v += t[i]
    while v_space:
        ans_v += '-'
        v_space.pop()

    return ans_u, ans_v

def main():
    m = int(input())
    s = [None for _ in range(m)]
    for i in range(m):
        s[i] = input()

    for i in range(m):
        for j in range(i+1, m):
            s[i], s[j] = local_align(s[i], s[j])

    for x in s:
        print(x)

if __name__ == '__main__':
    main()
