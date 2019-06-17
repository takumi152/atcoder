
def main():
    buf = input()
    buflist = buf.split()
    A = int(buflist[0])
    B = int(buflist[1])
    if B % A == 0:
        print(A+B)
    else:
        print(B-A)

if __name__ == '__main__':
    main()
