
def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    D = int(buflist[2])
    board = []
    for i in range(H):
        buf = input()
        buflist = buf.split()
        board.append(list(map(int, buflist)))
    buf = input()
    Q = int(buf)
    query = []
    for i in range(Q):
        buf = input()
        buflist = buf.split()
        L = int(buflist[0])
        R = int(buflist[1])
        query.append((L, R))
    moddivxy = []
    for i in range(H):
        for j in range(W):
            moddivxy.append((board[i][j] % D, board[i][j] // D, i, j))
    moddivxy = list(sorted(moddivxy))
    mp_table = [[0]]
    mod = 0
    last_xy = (-1, -1)
    for x in moddivxy:
        if x[0] == 0 and x[1] == 1: # first element
            mp_table[mod].append(0)
        elif x[0] > mod:
            mod = x[0]
            mp_table.append([0])
        else:
            mp_table[mod].append(mp_table[mod][-1] + abs(last_xy[0] - x[2]) + abs(last_xy[1] - x[3]))
        last_xy = (x[2], x[3])
    for q in query:
        m = q[0] % D
        d_start = q[0] // D
        d_end = q[1] // D
        print(mp_table[m][d_end] - mp_table[m][d_start])


if __name__ == '__main__':
    main()
