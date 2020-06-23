from collections import deque

def main():
    n, m, q = map(int, input().split())
    abcd = [None for _ in range(q)]
    for i in range(q):
        abcd[i] = list(map(int, input().split()))

    ans = 0
    que = deque()
    que.append([])
    while que:
        now = que.popleft()
        if len(now) < n:
            minimum = (now[-1] if now else 1)
            for i in range(minimum, m+1):
                next = list(now)
                next.append(i)
                que.append(next)
        else:
            score = 0
            for x in abcd:
                if now[x[1]-1] - now[x[0]-1] == x[2]:
                    score += x[3]
            if score > ans:
                ans = score

    print(ans)

if __name__ == '__main__':
    main()
