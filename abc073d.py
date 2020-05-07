import itertools

def main():
    n, m, q = map(int, input().split())
    r = list(map(int, input().split()))
    dist = [[10**18 for _ in range(n)] for _ in range(n)]
    for i in range(m):
        a, b, c = map(int, input().split())
        dist[a-1][b-1] = c
        dist[b-1][a-1] = c

    for k in range(n):
        for i in range(n):
            for j in range(n):
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    ans = 10 ** 18
    for p in itertools.permutations(r):
        total = 0
        for i in range(q-1):
            total += dist[p[i]-1][p[i+1]-1]
        ans = min(ans, total)

    print(ans)

if __name__ == '__main__':
    main()
