
def main():
    buf = input()
    buflist = buf.split()
    P = int(buflist[0])
    Q = int(buflist[1])
    R = int(buflist[2])
    print(min(P+Q, Q+R, R+P))


if __name__ == '__main__':
    main()
