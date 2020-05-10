def main():
    n, k = map(int, input().split())
    a = list(map(int, input().split()))

    cycle = []
    visited = [False for _ in range(n)]
    now = 0

    while True:
        cycle.append(now)
        visited[now] = True
        now = a[now]-1
        if (visited[now]):
            break


    t = len(cycle)
    cstart = 0
    for i in range(t):
        if cycle[i] == now:
            cstart = i
            break

    c = t - cstart

    if k < cstart:
        print(cycle[k]+1)
    else:
        print(cycle[cstart + (k - cstart) % c]+1)

if __name__ == '__main__':
    main()
