
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    buf = input()
    buflist = buf.split()
    T = int(buflist[0])
    A = int(buflist[1])
    buf = input()
    buflist = buf.split()
    H = list(map(int, buflist))

    mindiff = float("inf")
    point = None
    for i in range(N):
        temp = T - (H[i] * 0.006)
        diff = abs(A - temp)
        if diff < mindiff:
            mindiff = diff
            point = i
    print(point+1)

if __name__ == '__main__':
    main()
