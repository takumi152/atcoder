# TLE

import copy

def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    board = []
    for i in range(H):
        buf = input()
        board.append(list(buf))
    has_white = False
    for i in range(H):
        for j in range(W):
            if board[i][j] == '.':
                has_white = True
                break
        if has_white:
            break
    if not has_white: # all black
        print(0)
        return
    count = 0
    changed = True
    while(changed):
        count += 1
        changed = False
        new_board = []
        for i in range(H):
            new_board.append([])
            for j in range(W):
                new_board[i].append('.')
        for i in range(H):
            for j in range(W):
                if board[i][j] == '#':
                    new_board[i][j] = '#' #itself
                    if i > 0: #up
                        new_board[i-1][j] = '#'
                    if i < H-1: #down
                        new_board[i+1][j] = '#'
                    if j > 0: #left
                        new_board[i][j-1] = '#'
                    if j < W-1: #up
                        new_board[i][j+1] = '#'
        for i in range(H):
            for j in range(W):
                if not changed and new_board[i][j] == '.':
                    changed = True
                board[i][j] = new_board[i][j]
    print(count)

if __name__ == '__main__':
    main()
