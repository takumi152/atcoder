def main():
    n, m, q = map(int, input().split())
    edge = [[] for _ in range(n)]
    for i in range(m):
        u, v = map(int, input().split())
        edge[u-1].append(v-1)
        edge[v-1].append(u-1)
    c = list(map(int, input().split()))
    s = [None for _ in range(q)]
    for i in range(q):
        s[i] = list(map(int, input().split()))

    ans = [None for _ in range(q)]
    for i in range(q):
        ans[i] = c[s[i][1]-1]
        if s[i][0] == 1:
            for x in edge[s[i][1]-1]:
                c[x] = c[s[i][1]-1]
        else:
            c[s[i][1]-1] = s[i][2]

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
