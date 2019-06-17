
def main():
    buf = input()
    N = int(buf)
    AB = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        AB.append((int(buflist[0]), int(buflist[1])))
    for x in AB:
        print(x[0] * x[1])

if __name__ == '__main__':
    main()
