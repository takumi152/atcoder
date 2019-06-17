
def main():
    buf = input()
    N = int(buf)
    t = [0]
    x = [0]
    y = [0]
    for i in range(N):
        buf = input()
        buflist = buf.split()
        t.append(int(buflist[0]))
        x.append(int(buflist[1]))
        y.append(int(buflist[2]))
    for i in range(N):
        delta = abs(t[i] - t[i+1])
        dist = abs(x[i] - x[i+1]) + abs(y[i] - y[i+1])
        # odd / even check
        if (delta % 2 != dist % 2):
            print("No")
            return
        # reachability check
        if (delta < dist):
            print("No")
            return
    # all done
    print("Yes")

if __name__ == '__main__':
    main()
