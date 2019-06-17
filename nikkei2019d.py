import sys

def main():
    sys.setrecursionlimit(1000000) # recursion limit hack
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    M = int(buflist[1])
    node = []
    unpointed = set()
    for i in range(N):
        node.append(set())
        unpointed.add(i)
    for i in range(N-1+M):
        buf = input()
        buflist = buf.split()
        node[int(buflist[0])-1].add(int(buflist[1])-1)
    for i in range(N):
        for j in node[i]:
            unpointed.discard(j)
    root = unpointed.pop()
    recursion_graph(node, set(), root, root)
    ancestor = []
    for i in range(N):
        ancestor.append(None)
    recursion_ancestor(node, ancestor, root, root)
    for i in range(N):
        if ancestor[i] != None:
            print(ancestor[i]+1)
        else:
            print(0)

def recursion_graph(node, sub, current, last):
    sub = set(sub)
    for e in sub:
        if e[0] != last and e[1] == current:
            node[e[0]].discard(e[1])
    for dest in list(node[current]):
        sub.add((current, dest))
    for dest in list(node[current]):
        recursion_graph(node, sub, dest, current)

def recursion_ancestor(node, ancestor, current, last):
    if current != last:
        ancestor[current] = last
    for dest in list(node[current]):
        recursion_ancestor(node, ancestor, dest, current)

if __name__ == '__main__':
    main()
