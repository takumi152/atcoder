
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    H = int(buflist[1])
    katana = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        katana.append(int(buflist[0]), int(buflist[1]))

if __name__ == '__main__':
    main()
