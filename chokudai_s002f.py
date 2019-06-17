
def main():
    buf = input()
    N = int(buf)
    AB = set()
    for i in range(N):
        buf = input()
        buflist = buf.split()
        AB.add((min(int(buflist[0]), int(buflist[1])), max(int(buflist[0]), int(buflist[1]))))
    print(len(AB))

if __name__ == '__main__':
    main()
