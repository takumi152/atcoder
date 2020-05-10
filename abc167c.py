def main():
    n, m, x = map(int, input().split())
    c = [None for _ in range(n)]
    a = [None for _ in range(n)]
    for i in range(n):
        p = list(map(int, input().split()))
        c[i] = p[0]
        a[i] = p[1:m+1]

    ans = 10 ** 18
    for i in range(1 << n):
        mask = [False for _ in range(n)]
        p = i
        q = 0
        while p > 0:
            if p & 1:
                mask[q] = True
            p = p >> 1
            q += 1
        skill = [0 for _ in range(m)]
        cost = 0
        for j in range(n):
            if mask[j]:
                for k in range(m):
                    skill[k] += a[j][k]
                cost += c[j]
        good = True
        for j in range(m):
            if skill[j] < x:
                good = False
                break
        if good and cost < ans:
            ans = cost

    if ans == 10 ** 18:
        print(-1)
    else:
        print(ans)

if __name__ == '__main__':
    main()
