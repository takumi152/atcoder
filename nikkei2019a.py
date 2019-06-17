
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    A = int(buflist[1])
    B = int(buflist[2])
    print(min(A,B), max(0, (A+B)-N))

if __name__ == '__main__':
    main()
