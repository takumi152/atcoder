
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
        total += min(x[0] // 2, x[1])
    print(total)

if __name__ == '__main__':
    main()
