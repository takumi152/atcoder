# WA

EPS = 1e-9

def main():
    buf = input()
    buflist = buf.split()
    W = int(buflist[0])
    H = int(buflist[1])
    x = int(buflist[2])
    y = int(buflist[3])
    area = []
    multiple = False
    # LR
    area.append(float(min(W * y, W * (H - y))))
    # UD
    area.append(float(min(x * H, (W - x) * H)))
    # diag LD
    if x + y <= H:
        if x + y <= W:
            area.append(min((x + y) * (x + y) / 2, W * H - ((x + y) * (x + y) / 2)))
        else:
            area.append(min(((x + y) + (x + y - W)) * W / 2, W * H - ((x + y) + (x + y - W)) * W / 2))
    else:
        if x + y <= W:
            area.append(min(((x + y - H) + (x + y)) * H / 2, W * H - ((x + y - H) + (x + y)) * H / 2))
        else:
            area.append(min((2 * H - x + y) * (2 * W - x + y) / 2, W * H - ((2 * H - x + y) * (2 * W - x + y) / 2)))
    # diag LU
    W, H, x, y = H, W, y, W - x
    if x + y <= H:
        if x + y <= W:
            area.append(min((x + y) * (x + y) / 2, W * H - ((x + y) * (x + y) / 2)))
        else:
            area.append(min(((x + y) + (x + y - W)) * W / 2, W * H - ((x + y) + (x + y - W)) * W / 2))
    else:
        if x + y <= W:
            area.append(min(((x + y - H) + (x + y)) * H / 2, W * H - ((x + y - H) + (x + y)) * H / 2))
        else:
            area.append(min((2 * H - x + y) * (2 * W - x + y) / 2, W * H - ((2 * H - x + y) * (2 * W - x + y) / 2)))
    area.sort()
    if area[-1] - area[-2] < EPS:
        multiple = True
    print(area[-1], 1 if multiple else 0)


if __name__ == '__main__':
    main()
