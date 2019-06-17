
def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    board = []
    for i in range(H):
        buf = input()
        board.append(buf)
    # passage check
    grouping = []
    for i in range(H):
        grouping.append([])
        for j in range(W):
            grouping[i].append(0)
    group_number = 1
    for i in range(H):
        if grouping[i][0] != 0:
            continue
        if board[i][0] == '#':
            continue
        group_coloring(board, grouping, i, group_number, H, W)
        group_number += 1
    passage_list = []
    for i in range(H):
        passage = True
        for j in range(W):
            if grouping[i][j] == 0:
                passage = False
                break
        if passage:
            passage_list.append(i)
    if not passage_list:
        print(':(') # fail
        return
    # reachability check
    if grouping[H-1][W-1] == 0:
        print(':(') # fail
        return
    print('Yay!') # success

def group_coloring(board, grouping, start, group_number, H, W):
    stack = [[start, 0]]
    while stack:
        point = stack.pop()
        if board[point[0]][point[1]] == '#' or grouping[point[0]][point[1]] != 0:
            continue
        grouping[point[0]][point[1]] = group_number
        # down
        if point[0] < H-1:
            stack.append([point[0]+1, point[1]])
        # right
        if point[1] < W-1:
            stack.append([point[0], point[1]+1])

if __name__ == '__main__':
    main()
