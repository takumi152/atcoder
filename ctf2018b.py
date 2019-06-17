
def main():
    buf = input()
    buflist = buf.split()
    X = int(buflist[0])
    Y = int(buflist[1])
    if X >= Y:
        pass
    else:
        temp = X
        X = Y
        Y = temp
    a = Y * 3
    b = Y
    if a < X:
        print("No")
        return
    elif a == X:
        print("Yes")
        return
    elif (a - X) % 8 == 0:
        print("Yes")
        return
    else:
        print("No")
        return



if __name__ == '__main__':
    main()
