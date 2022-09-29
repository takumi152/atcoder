mod = int(1e9) + 7

def modmatmul(a, b, mod = mod):
    p = len(a)
    q = len(a[0])
    r = len(b[0])
    res = [[0 for _ in range(r)] for _ in range(p)]
    for i in range(p):
        for k in range(r):
            for j in range(q):
                res[i][k] = (res[i][k] + a[i][j] * b[j][k]) % mod
    return res


def main():
    n, m, k = map(int, input().split())
    a = list(map(int, input().split()))
    xy = [None for _ in range(m)]
    for i in range(m):
        xy[i] = list(map(int, input().split()))
        xy[i][0] -= 1
        xy[i][1] -= 1

    c = []
    for x in a:
        c.append([x])

    b = [[0 for _ in range(n)] for _ in range(n)]
    for i in range(n):
        b[i][i] = m * 2
    for j in range(m):
        b[xy[j][0]][xy[j][0]] -= 1
        b[xy[j][1]][xy[j][1]] -= 1
        b[xy[j][0]][xy[j][1]] += 1
        b[xy[j][1]][xy[j][0]] += 1

    t = 1
    d = 2 * m
    denom = 1
    while t <= k:
        # print(t, d, denom)
        if k & t:
            c = modmatmul(b, c)
            for i in range(n):
                c[i][0] = c[i][0] % mod
            denom = (denom * d) % mod
        b = modmatmul(b, b)
        for i in range(n):
            for j in range(n):
                b[i][j] = b[i][j] % mod
        # print(t, d, denom)
        d = (d * d) % mod
        t <<= 1

    # print(c)

    ans = [0 for _ in range(n)]
    for i in range(n):
        ans[i] = (c[i][0] * pow(denom, mod-2, mod)) % mod

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
