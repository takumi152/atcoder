
def main():
    buf = input()
    buflist = buf.split()
    A = int(buflist[0])
    P = int(buflist[1])
    P += A * 3
    print(P // 2)

if __name__ == '__main__':
    main()
