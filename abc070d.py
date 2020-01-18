
def main():
    n = int(input())
    edge = []
    for i in range(n):
        edge.append([])
    for i in range(n-1):
        abc = list(map(int, input().split()))
        edge[abc[0]-1].append((abc[1]-1, abc[2])) # zero index
        edge[abc[1]-1].append((abc[0]-1, abc[2])) # zero index
    q, k = map(int, input().split())
    k -= 1 # zero index
    xy = []
    for i in range(q):
        xy.append(list(map(int, input().split())))
        xy[-1][0] -= 1 # zero index
        xy[-1][1] -= 1 # zero index

    distance_from_k = []
    visited = []
    for i in range(n):
        distance_from_k.append(0)
        visited.append(False)
    st = [k]
    while st:
        now = st.pop()
        if visited[now]:
            continue
        visited[now] = True
        for x in edge[now]:
            if visited[x[0]]:
                continue
            distance_from_k[x[0]] = distance_from_k[now] + x[1]
            st.append(x[0])

    ans = []
    for i in range(q):
        ans.append(distance_from_k[xy[i][0]] + distance_from_k[xy[i][1]])

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
