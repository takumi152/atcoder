
def main():
    buf = input()
    buflist = buf.split()
    N = int(buflist[0])
    K = int(buflist[1])
    print((N+1)-K)

if __name__ == '__main__':
    main()
