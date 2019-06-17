
def main():
    buf = input()
    buflist = buf.split()
    X = int(buflist[0])
    Y = int(buflist[1])
    print(X + int(Y / 2))

if __name__ == '__main__':
    main()
