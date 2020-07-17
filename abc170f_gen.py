import random

def main():
    h = 1000
    w = 1000
    k = 1000000

    board = [['.' for _ in range(w)] for _ in range(h)]
    for i in range(h):
        for j in range(w):
            if (i == 0 and j == 0) or (i == h-1 and j == w-1):
                continue
            if random.random() < 0.1:
                board[i][j] = '@'

    print(h, w, k)
    print(1, 1, h, w)
    for i in range(h):
        print(''.join(board[i]))


if __name__ == '__main__':
    main()
