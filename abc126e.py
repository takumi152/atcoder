
def main():
    buf = input()
    buflist = list(buf.split())
    N = int(buflist[0])
    M = int(buflist[1])
    edge = dict()
    for i in range(N):
        edge[i] = [] # zero index
    for i in range(M):
        buf = input()
        buflist = list(buf.split())
        X = int(buflist[0])-1 # zero index
        Y = int(buflist[1])-1 # zero index
        Z = int(buflist[2])
        edge[X].append(Y)
        edge[Y].append(X)
    seen = []
    for i in range(N):
        seen.append(False)
    group_num = 0
    for i in range(N):
        if seen[i]:
            continue
        group_num += 1
        seen[i] = True
        stack = []
        for x in edge[i]:
            stack.append(x)
        while stack:
            current = stack.pop()
            if seen[current]:
                continue
            seen[current] = True
            for x in edge[current]:
                if not seen[x]:
                    stack.append(x)
    print(group_num)

if __name__ == '__main__':
    main()
