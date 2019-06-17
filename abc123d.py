import heapq

def main():
    buf = input()
    buflist = buf.split()
    X = int(buflist[0])
    Y = int(buflist[1])
    Z = int(buflist[2])
    K = int(buflist[3])
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    B = list(map(int, buflist))
    buf = input()
    buflist = buf.split()
    C = list(map(int, buflist))
    A = list(sorted(A, reverse=True))
    B = list(sorted(B, reverse=True))
    C = list(sorted(C, reverse=True))
    cakeheap = [(-(A[0]+B[0]+C[0]), 0, 0, 0)]
    hashtable = {(-(A[0]+B[0]+C[0]), 0, 0, 0)}
    tastiness = []
    while len(tastiness) < K:
        current = heapq.heappop(cakeheap)
        tastiness.append(-current[0])
        if current[1] < len(A)-1:
            if (-(A[current[1]+1]+B[current[2]]+C[current[3]]), current[1]+1, current[2], current[3]) not in hashtable:
                heapq.heappush(cakeheap, (-(A[current[1]+1]+B[current[2]]+C[current[3]]), current[1]+1, current[2], current[3]))
                hashtable.add((-(A[current[1]+1]+B[current[2]]+C[current[3]]), current[1]+1, current[2], current[3]))
        if current[2] < len(B)-1:
            if (-(A[current[1]]+B[current[2]+1]+C[current[3]]), current[1], current[2]+1, current[3]) not in hashtable:
                heapq.heappush(cakeheap, (-(A[current[1]]+B[current[2]+1]+C[current[3]]), current[1], current[2]+1, current[3]))
                hashtable.add((-(A[current[1]]+B[current[2]+1]+C[current[3]]), current[1], current[2]+1, current[3]))
        if current[3] < len(C)-1:
            if (-(A[current[1]]+B[current[2]]+C[current[3]+1]), current[1], current[2], current[3]+1) not in hashtable:
                heapq.heappush(cakeheap, (-(A[current[1]]+B[current[2]]+C[current[3]+1]), current[1], current[2], current[3]+1))
                hashtable.add((-(A[current[1]]+B[current[2]]+C[current[3]+1]), current[1], current[2], current[3]+1))
    for i in tastiness:
        print(i)


if __name__ == '__main__':
    main()
