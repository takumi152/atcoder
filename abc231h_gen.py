import random

def main():
    h = 31
    w = 31
    n = h * w

    board = [[None for _ in range(w)] for _ in range(h)]
    for i in range(h):
        for j in range(w):
            board[i][j] = random.randint(9 * 10 ** 8, 10 ** 9)

    print(h, w, n)
    for i in range(h):
        for j in range(w):
            if board[i][j] is not None:
                print(i+1, j+1, board[i][j])

if __name__ == '__main__':
    main()
