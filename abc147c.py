def main():
    n = int(input())
    a = [0 for _ in range(n)]
    xy = [None for _ in range(n)]
    for i in range(n):
        a[i] = int(input())
        xy[i] = [None for _ in range(a[i])]
        for j in range(a[i]):
            xy[i][j] = tuple(map(int, input().split()))

    ans = 0
    for i in range(1 << n):
        m = [0 for _ in range(n)]
        p = 0
        for j in range(n):
            m[j] = (1 if i & (1 << j) else 0)
            p += (1 if i & (1 << j) else 0)
        if p < ans:
            continue
        good = True
        for j in range(n):
            if m[j]:
                for k in range(a[j]):
                    if m[xy[j][k][0]-1] != xy[j][k][1]:
                        good = False
                        break
            if not good:
                break
        if good:
            ans = p

    print(ans)


if __name__ == '__main__':
    main()
