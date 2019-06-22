
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    K = int(buflist[1])
    edge = dict()
    for i in range(N):
        edge[i] = set()
        for j in range(i+1, N):
            edge[i].add(j)
    removed = 0
    for i in range(N):
        for j in range(i+1, N-1):
            if removed < K:
                edge[i].remove(j)
                removed += 1
    if removed < K:
        print(-1) # impossible
        return
    print((N * (N - 1)) // 2 - K)
    for i in range(N):
        for x in edge[i]:
            print(i+1, x+1)

if __name__ == '__main__':
    main()
