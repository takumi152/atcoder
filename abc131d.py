
def main():
    buf = input()
    N = int(buf)
    BA = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        BA.append((int(buflist[1]), int(buflist[0])))
    BA.sort()
    elapsed = 0
    for x in BA:
        elapsed += x[1]
        if elapsed > x[0]:
            print('No')
            return
    print('Yes')

if __name__ == '__main__':
    main()
