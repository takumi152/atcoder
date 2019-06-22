# WIP

def main():
    buf = input()
    N = int(buf)
    xy = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        xy.append((int(buflist[0]), int(buflist[1])))
    xd = dict()
    yd = dict()
    for p in xy:
        if p[0] not in xd:
            xd[p[0]] = {p[1]}
        else:
            xd[p[0]].add(p[1])
        if p[1] not in yd:
            yd[p[0]] = {p[1]}
        else:
            yd[p[0]].add(p[1])

if __name__ == '__main__':
    main()
