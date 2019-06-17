
def main():
    buf = input()
    N = int(buf)
    SP = []
    for i in range(N):
        buf = input()
        buflist = buf.split()
        SP.append((buflist[0], -int(buflist[1]), i+1))
    SP = list(sorted(SP))
    for x in SP:
        print(x[2])

if __name__ == '__main__':
    main()
