
def main():
    buf = input()
    N = int(buf)
    AB = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        AB.append((int(buflist[0]), int(buflist[1])))
    total = 0
    for x in AB:
        if x[0] > x[1]:
            total += x[0]
        else:
            total += x[1]
    print(total)

if __name__ == '__main__':
    main()
