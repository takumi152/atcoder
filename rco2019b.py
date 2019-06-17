import time
import random
import math
from collections import deque

def main():
    t_start = time.time()
    buf = input()
    buflist = buf.split()
    N = int(buflist[0]) # = 50
    M = int(buflist[1]) # = 2500
    garden = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        garden.append(list(map(int, buflist)))
    remaining = M
    map9 = []
    for i in range(N):
        map9.append([])
        for j in range(N):
            map9[i].append(True if garden[i][j] == 9 else False)
    # find nearest from center
    nearest = [-1, -1]
    for i in range(N):
        for j in range(N):
            if map9[i][j]:
                if abs(nearest[0] - N // 2) + abs(nearest[1] - N // 2) > abs(i - N // 2) + abs(j - N // 2):
                    nearest = [i, j]
    # make 9s until limit
    q = deque([[nearest[0]-1, nearest[1]], [nearest[0]+1, nearest[1]], [nearest[0], nearest[1]-1], [nearest[0], nearest[1]+1]])
    threshold = 8
    loop_remaining = len(q)
    while remaining >= 9 and len(q) > 0:
        a = q.popleft()
        loop_remaining -= 1
        if a[0] >= 0 and a[0] < N and a[1] >= 0 and a[1] < N:
            if not map9[a[0]][a[1]]:
                if garden[a[0]][a[1]] >= threshold:
                    for i in range(9-garden[a[0]][a[1]]):
                        print(1, a[0], a[1])
                        remaining -= 1
                    map9[a[0]][a[1]] = True
                    q += [[a[0]-1, a[1]], [a[0]+1, a[1]], [a[0], a[1]-1], [a[0], a[1]+1]]
                    threshold = 8
                    loop_remaining = len(q)
                else:
                    q.append(a)
        if loop_remaining <= 0:
            threshold -= 1
            loop_remaining = len(q)
    print(2, nearest[0], nearest[1])

if __name__ == '__main__':
    main()
