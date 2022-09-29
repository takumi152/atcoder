def main():
    h, w = map(int, input().split())
    c = [None for _ in range(h)]
    for i in range(h):
        c[i] = list(input())

    dx = (-1, 1, 0, 0)
    dy = (0, 0, -1, 1)

    for i in range(h):
        for j in range(w):
            if c[i][j] == '.':
                existence = [False for _ in range(5)]
                for d in range(4):
                    if i + dx[d] >= 0 and j + dy[d] >= 0 and i + dx[d] < h and j + dy[d] < w:
                        if c[i + dx[d]][j + dy[d]] != '.':
                            existence[int(c[i + dx[d]][j + dy[d]]) - 1] = True
                for k in range(5):
                    if not existence[k]:
                        c[i][j] = str(k + 1)
                        break

    for i in range(h):
        print(''.join(c[i]))

if __name__ == '__main__':
    main()
