def main():
    n, k = map(int, input().split())
    d = [None for _ in range(k)]
    a = [None for _ in range(k)]
    for i in range(k):
        d[i] = int(input())
        a[i] = list(map(int, input().split()))

    tricked = [True for _ in range(n)]
    for i in range(k):
        for j in range(d[i]):
            tricked[a[i][j]-1] = False

    ans = 0
    for i in range(k):
        if tricked[i]:
            ans += 1

    print(ans)

if __name__ == '__main__':
    main()
