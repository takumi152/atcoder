
def main():
    buf = input()
    buflist = buf.split()
    A = int(buflist[0])
    B = int(buflist[1])
    if A >= 13:
        print(B)
    elif A >= 6:
        print(B // 2)
    else:
        print(0)

if __name__ == '__main__':
    main()
