
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    T = int(buflist[1])
    c = []
    t = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        c.append(int(buflist[0]))
        t.append(int(buflist[1]))
    min = None
    index = None
    for i in range(N):
        if t[i] <= T:
            if min == None:
                min = c[i]
                index = i
            elif c[i] < min:
                min = c[i]
                index = i
    if min == None:
        print("TLE")
    else:
        print(min)

if __name__ == '__main__':
    main()
