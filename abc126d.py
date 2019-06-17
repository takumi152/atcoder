
def main():
    buf = input()
    N = int(buf)
    edge = dict()
    for i in range(N):
        edge[i] = [] # zero index
    for i in range(N-1):
        buf = input()
        buflist = list(buf.split())
        u = int(buflist[0])-1 # zero index
        v = int(buflist[1])-1 # zero index
        w = int(buflist[2])
        edge[u].append((w, v))
        edge[v].append((w, u))
    vertex_color = []
    for i in range(N):
        vertex_color.append(None)
    stack = [(0, 0)] # distance, vertex_id
    while stack:
        current = stack.pop()
        if current[0] % 2 == 0:
            vertex_color[current[1]] = 1
        else:
            vertex_color[current[1]] = 0
        for e in edge[current[1]]:
            if vertex_color[e[1]] == None:
                stack.append((current[0]+e[0], e[1]))
    for x in vertex_color:
        print(x)

if __name__ == '__main__':
    main()
