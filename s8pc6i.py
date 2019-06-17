
def main():
    buf = input()
    buflist = buf.split()
    H = int(buflist[0])
    W = int(buflist[1])
    board = []
    for i in range(H):
        buf = input()
        buflist = buf.split()
        board.append(list(map(int, buflist)))
    for i in range(H):
        if i % 3 == 0:
            print('#' * W)
        elif i % 3 == 1:
            if i == H - 1:
                sum_top = 0
                sum_bottom = 0
                for j in range(2, W):
                    if j % 2 == 0:
                        sum_top += board[i][j]
                    else:
                        sum_bottom += board[i][j]
                if sum_top > sum_bottom:
                    print(('#.' * (W // 2 + 1))[:W])
                else:
                    print(('#.' + '.#' * (W // 2))[:W])
            else:
                sum_top = 0
                sum_bottom = 0
                for j in range(2, W):
                    if j % 2 == 0:
                        sum_top += board[i][j]
                        sum_bottom += board[i+1][j]
                    else:
                        sum_bottom += board[i][j]
                        sum_top += board[i+1][j]
                if sum_top > sum_bottom:
                    print(('#.' * (W // 2 + 1))[:W])
                    print(('#.' + '.#' * (W // 2))[:W])
                else:
                    print(('#.' + '.#' * (W // 2))[:W])
                    print(('#.' * (W // 2 + 1))[:W])
        else:
            continue

if __name__ == '__main__':
    main()
