# TLE

from bisect import bisect_left

def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    Q = int(buflist[1])
    XST = dict()
    for i in range(N):
        buf = input()
        buflist = buf.split()
        X = int(buflist[2])
        ST = (int(buflist[0]) - X, int(buflist[1]) - X)
        if X not in XST:
            XST[X] = [ST]
        else:
            XST[X].append(ST)
    Ds = set()
    Dl = []
    for i in range(Q):
        buf = input()
        D = int(buf)
        Ds.add(D)
        Dl.append(D)
    distance = []
    for i in range(Q):
        distance.append(-1)
    for i in sorted(XST):
        for x in XST[i]:
            start = bisect_left(Dl, x[0])
            point = start
            while point < Q:
                if Dl[point] >= x[1]:
                    break
                if distance[point] == -1:
                    distance[point] = i
                point += 1
    for x in distance:
        print(x)


if __name__ == '__main__':
    main()
