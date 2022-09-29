def main():
    h, w, x, y = map(int, input().split())
    s = ['' for _ in range(h)]
    for i in range(h):
        s[i] = input()

    dx = [-1, 1, 0, 0]
    dy = [0, 0, -1, 1]

    ans = 1
    for i in range(4):
        px = (x - 1) + dx[i]
        py = (y - 1) + dy[i]
        while px >= 0 and py >= 0 and px < h and py < w:
            if s[px][py] == '#':
                break
            ans += 1
            px += dx[i]
            py += dy[i]

    print(ans)

if __name__ == '__main__':
    main()
