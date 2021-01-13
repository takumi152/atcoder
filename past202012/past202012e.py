def rotate_90_deg_ccw(a):
    a_next = ['' for _ in range(len(a[0]))]
    for i in range(len(a)):
        for j in range(len(a[i])):
            a_next[j] += a[i][j]
    a_next.reverse()
    return a_next

def main():
    h, w = map(int, input().split())
    s = ['' for _ in range(h)]
    for i in range(h):
        s[i] = input()
    t = ['' for _ in range(h)]
    for i in range(h):
        t[i] = input()

    for r in range(4):
        for i in range(r):
            t = rotate_90_deg_ccw(t)

        if r % 2 == 0:
            for i in range(-10, 10):
                for j in range(-10, 10):
                    good = True
                    for k in range(h):
                        for l in range(w):
                            if t[k][l] == '.':
                                continue
                            elif i+k < 0 or i+k >= h or j+l < 0 or j+l >= w:
                                good = False
                                break
                            elif s[i+k][j+l] == '#':
                                good = False
                                break
                    if good:
                        print('Yes')
                        return
        else:
            for i in range(-10, 10):
                for j in range(-10, 10):
                    good = True
                    for k in range(w):
                        for l in range(h):
                            if t[k][l] == '.':
                                continue
                            elif i+k < 0 or i+k >= h or j+l < 0 or j+l >= w:
                                good = False
                                break
                            elif s[i+k][j+l] == '#':
                                good = False
                                break
                    if good:
                        print('Yes')
                        return

        for i in range(4-r):
            t = rotate_90_deg_ccw(t)

    print('No')

if __name__ == '__main__':
    main()
