
def main():
    buf = input()
    buflist = buf.split()
    sx = int(buflist[0])
    sy = int(buflist[1])
    tx = int(buflist[2])
    ty = int(buflist[3])
    xdiff = tx - sx
    ydiff = ty - sy
    movement = ""
    movement += "R" * xdiff
    movement += "U" * ydiff
    movement += "L" * xdiff
    movement += "D" * ydiff
    movement += "D"
    movement += "R" * (xdiff+1)
    movement += "U" * (ydiff+1)
    movement += "L"
    movement += "U"
    movement += "L" * (xdiff+1)
    movement += "D" * (ydiff+1)
    movement += "R"
    print(movement)

if __name__ == '__main__':
    main()
