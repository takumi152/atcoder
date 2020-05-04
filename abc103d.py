import heapq

def main():
    n, m = map(int, input().split())
    ab = [None] * m
    for i in range(m):
        ab[i] = list(map(int, input().split()))

    ab.sort()

    que = []
    cut_front = 0
    for x in ab:
        if que:
            if que[0] <= x[0]:
                cut_front += 1
                que.clear()
        heapq.heappush(que, x[1])
    if que:
        cut_front += 1
        que.clear()

    ab.sort(reverse=True, key=lambda x: x[1])

    cut_back = 0
    for x in ab:
        if que:
            if x[1] <= -que[0]:
                cut_back += 1
                que.clear()
        heapq.heappush(que, -x[0])
    if que:
        cut_back += 1
        que.clear()

    print(min(cut_front, cut_back))

if __name__ == '__main__':
    main()
