def main():
    n = int(input())
    ab = [None for _ in range(n)]
    for i in range(n):
        ab[i] = list(map(int, input().split()))

    event = []
    for i in range(n):
        event.append((ab[i][0], 1))
        event.append((ab[i][0] + ab[i][1], -1))
    event.sort()

    login = 0
    ans = [0 for _ in range(n)]
    for i, e in enumerate(event):
        login += e[1]
        if i < len(event) - 1 and login > 0:
            ans[login-1] += event[i+1][0] - e[0]

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
