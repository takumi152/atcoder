
def main():
    buf = input()
    buflist = buf.split()
    N = list(map(int, buflist))
    if 1 in N and 9 in N and 7 in N and 4 in N:
        print("YES")
    else:
        print("NO")

if __name__ == '__main__':
    main()
