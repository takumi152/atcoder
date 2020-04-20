from collections import deque

def main():
    n = int(input())
    a = []
    for i in range(n):
        a.append(list(map(int, input().split())))

    que = deque()
    lastdone = []
    next = []
    for i in range(n):
        que.append((i, 1))
        lastdone.append(-1)
        next.append(0)
    while len(que) > 0:
        now, day = que.popleft()
        if lastdone[now] == day:
            continue
        if next[now] == n-1:
            continue
        candidate = a[now][next[now]]-1
        if lastdone[candidate] == day:
            continue
        if next[candidate] == n-1:
            continue
        if a[candidate][next[candidate]]-1 == now:
            next[now] += 1
            next[candidate] += 1
            lastdone[now] = day
            lastdone[candidate] = day
            que.append((now, day+1))
            que.append((candidate, day+1))

    for i in range(n):
        if next[i] != n-1:
            print(-1)
            return

    print(max(lastdone))

if __name__ == '__main__':
    main()
