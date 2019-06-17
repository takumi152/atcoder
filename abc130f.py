# WIP

def main():
    buf = input()
    N = int(buf)
    xyd = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        xyd.append((int(buflist[0]), int(buflist[1]), buflist[2]))
    up = []
    down = []
    left = []
    right = []
    for x in xyd:
        if x[2] == 'U':
            up.append(x)
        elif x[2] == 'D':
            down.append(x)
        elif x[2] == 'L':
            left.append(x)
        else: # x[2] == 'R'
            right.append(x)
    up.sort(key = lambda x: x[0])
    down.sort(key = lambda x: x[0])
    left.sort(key = lambda x: x[1])
    right.sort(key = lambda x: x[1])
    up_const = (up[0][0], up[-1][0]) if up else None
    down_const = (down[0][0], down[-1][0]) if down else None
    left_const = (left[0][1], left[-1][1]) if left else None
    right_const = (right[0][1], right[-1][1]) if right else None
    up.sort(key = lambda x: x[1])
    down.sort(key = lambda x: x[1])
    left.sort(key = lambda x: x[0])
    right.sort(key = lambda x: x[0])
    up_range = (up[0][1], up[-1][1]) if up else None
    down_range = (down[0][1], down[-1][1]) if down else None
    left_range = (left[0][0], left[-1][0]) if left else None
    right_range = (right[0][0], right[-1][0]) if right else None
    print(up)
    print(down)
    print(left)
    print(right)
    print(up_const)
    print(down_const)
    print(left_const)
    print(right_const)
    print(up_range)
    print(down_range)
    print(left_range)
    print(right_range)

if __name__ == '__main__':
    main()
