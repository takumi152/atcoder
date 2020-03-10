from collections import deque

def main():
    n = int(input())
    edge = []
    edge_id = []
    for i in range(n):
        edge.append([])
        edge_id.append(dict())
    for i in range(n-1):
        a, b = map(int, input().split())
        edge[a-1].append(b-1)
        edge[b-1].append(a-1)
        edge_id[a-1].update({b-1: i})
        edge_id[b-1].update({a-1: i})
    m = int(input())
    uv = []
    for i in range(m):
        uv.append(list(map(int, input().split())))

    edge_mask = []
    for i in range(m):
        distance_from_u = []
        visited = []
        for j in range(n):
            distance_from_u.append(0)
            visited.append(False)
        que = deque()
        que.append([uv[i][0]-1, 0])
        while len(que) > 0:
            now, dist = que.popleft()
            if visited[now]:
                continue
            visited[now] = True
            distance_from_u[now] = dist
            for x in edge[now]:
                if not visited[x]:
                    que.append([x, dist+1])
        next = uv[i][1]-1
        edge_set = set()
        while distance_from_u[next] > 0:
            for x in edge[next]:
                if distance_from_u[x] < distance_from_u[next]:
                    edge_set.add(edge_id[next][x])
                    next = x
                    break
        edge_mask.append(edge_set)

    dp_mask = []
    for i in range(n-1):
        new_mask = 0
        for j in range(m):
            if i in edge_mask[j]:
                new_mask = new_mask | (1 << j)
        dp_mask.append(new_mask)

    dp = []
    for i in range(n):
        dp.append([])
        for j in range(1 << m):
            dp[i].append(0)
    dp[0][0] = 1
    for i in range(n-1):
        for j in range(1 << m):
            dp[i+1][j] += dp[i][j]
            dp[i+1][j|dp_mask[i]] += dp[i][j]

    ans = dp[n-1][(1<<m)-1]

    print(ans)

if __name__ == '__main__':
    main()
