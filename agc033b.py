def main():
    h, w, n = map(int, input().split())
    sr, sc = map(int, input().split())
    s = input()
    t = input()

    # LR
    left_limit = 0
    right_limit = w
    for i in range(n-1, -1, -1):
        if t[i] == 'L' and right_limit < w:
            right_limit += 1
        elif t[i] == 'R' and left_limit > 0:
            left_limit -= 1
        if s[i] == 'L':
            left_limit += 1
        elif s[i] == 'R':
            right_limit -= 1
        if left_limit == right_limit:
            print('NO')
            return

    if sc <= left_limit or sc > right_limit:
        print('NO')
        return

    # UD
    up_limit = 0
    down_limit = h
    for i in range(n-1, -1, -1):
        if t[i] == 'U' and down_limit < h:
            down_limit += 1
        elif t[i] == 'D' and up_limit > 0:
            up_limit -= 1
        if s[i] == 'U':
            up_limit += 1
        elif s[i] == 'D':
            down_limit -= 1
        if up_limit == down_limit:
            print('NO')
            return

    if sr <= up_limit or sr > down_limit:
        print('NO')
        return

    print('YES')

if __name__ == '__main__':
    main()
