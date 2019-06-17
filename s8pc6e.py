import copy

UP = 0
RIGHT = 1
DOWN = 2
LEFT = 3
ANY = -1

def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    board = []
    for i in range(H):
        buf = input()
        board.append(list(buf))
    coin = 0
    for i in range(H):
        for j in range(W):
            if board[i][j] == 'o':
                coin += 1
    stack = []
    for i in range(H):
        for j in range(W):
            if board[i][j] == 'o':
                stack.append([i, j, ANY, coin-1, copy.deepcopy(board)])
                stack[-1][4][i][j] = 'x'
    while stack:
        state = stack.pop()
        if state[3] == 0:
            print('Possible')
            return
        if state[2] not in (UP, DOWN):
            for i in range(state[0], -1, -1):
                if state[4][i][state[1]] == 'o':
                    stack.append([i, state[1], UP, state[3]-1, copy.deepcopy(state[4])])
                    stack[-1][4][i][state[1]] = 'x'
        if state[2] not in (UP, DOWN):
            for i in range(state[0], H):
                if state[4][i][state[1]] == 'o':
                    stack.append([i, state[1], DOWN, state[3]-1, copy.deepcopy(state[4])])
                    stack[-1][4][i][state[1]] = 'x'
        if state[2] not in (LEFT, RIGHT):
            for j in range(state[1], -1, -1):
                if state[4][state[0]][j] == 'o':
                    stack.append([state[0], j, LEFT, state[3]-1, copy.deepcopy(state[4])])
                    stack[-1][4][state[0]][j] = 'x'
        if state[2] not in (LEFT, RIGHT):
            for j in range(state[0], H):
                if state[4][state[0]][j] == 'o':
                    stack.append([state[0], j, RIGHT, state[3]-1, copy.deepcopy(state[4])])
                    stack[-1][4][state[0]][j] = 'x'
    print('Impossible')

if __name__ == '__main__':
    main()
