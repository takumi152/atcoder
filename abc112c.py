
def main():
    buf = input()
    N = int(buf)
    x = []
    y = []
    h = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        x.append(int(buflist[0]))
        y.append(int(buflist[1]))
        h.append(int(buflist[2]))
    x = [z for _, z in sorted(zip(h,x), key=lambda pair: pair[0])][::-1]
    y = [z for _, z in sorted(zip(h,y), key=lambda pair: pair[0])][::-1]
    h = sorted(h)[::-1]
    for cx in range(101):
        for cy in range(101):
            H = h[0] + dist(cx, cy, x[0], y[0])
            correct = True
            for i in range(1, N):
                if h[i] + dist(cx, cy, x[i], y[i]) != H:
                    if dist(cx, cy, x[i], y[i]) <= H:
                        correct = False
                        break
                    else:
                        if h[i] != 0:
                            correct = False
                            break
            if correct == True:
                print(cx, cy, H)
                return


def dist(x1, y1, x2, y2):
    return abs(x1 - x2) + abs(y1 - y2)

if __name__ == '__main__':
    main()
