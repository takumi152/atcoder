
def main():
    buf = input()
    buflist = buf.split()
    X = int(buflist[0])
    A = int(buflist[1])
    if X < A:
        print(0)
    else:
        print(10)

if __name__ == '__main__':
    main()
