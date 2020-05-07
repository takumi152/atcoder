# WIP

import heapq

def main():
    q = int(input())
    query = [None for _ in range(q)]
    for i in range(q):
        query[i] = list(map(int, input().split()))

    left = [-q[0][1]]
    right = []
    f = 0
    x = -left[0]
    b = q[0][2]
    for i in range(1, q):
        if q[i][0] == 1:
            if q[i][1] <= x:
                heapq.heappush(left, -q[i][1])
            else:
                heapq.heappush(right, q[i][1])
            if len(left) > len(right)+1:
                heapq.heappush(right, -heapq.heappop(left))
            elif len(left) < len(right):
                heapq.heappush(left, -heapq.heappop(right))
            x = -left[0]
            b += q[i][2]
        elif q[i][0] == 2:
            print(x, f + b)


if __name__ == '__main__':
    main()
