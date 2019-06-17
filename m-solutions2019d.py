import sys

def main():
    sys.setrecursionlimit(1000000) # recursion limit hack
    buf = input()
    N = int(buf)
    edge = dict()
    ab = []
    for i in range(N):
        edge[i] = [] # zero index
    for i in range(N-1):
        buf = input()
        buflist = list(buf.split())
        a = int(buflist[0])-1 # zero index
        b = int(buflist[1])-1 # zero index
        edge[a].append(b)
        edge[b].append(a)
        ab.append((a, b))
    buf = input()
    buflist = buf.split()
    c = list(map(int, buflist))
    c = list(sorted(c, reverse=True))
    point = 0
    assigned = []
    weight = []
    for i in range(N):
        assigned.append(False)
        weight.append(0)
    start = None
    for k, v in edge.items():
        if len(v) == 1:
            start = k
            break
    recursion_graph(point, start, edge, c, weight, assigned)
    total = 0
    for x in ab:
        total += min(weight[x[0]], weight[x[1]])
    print(total)
    print(' '.join(list(map(str, weight))))

def recursion_graph(point, k, edge, c, weight, assigned):
    weight[k] = c[point]
    assigned[k] = True
    point += 1
    for node in edge[k]:
        if not assigned[node]:
            point = recursion_graph(point, node, edge, c, weight, assigned)
    return point

if __name__ == '__main__':
    main()
