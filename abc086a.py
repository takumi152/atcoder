
def main():
    buf = input()
    buflist = buf.split()
    buflist = list(map(int, buflist))
    if (buflist[0] * buflist[1]) % 2 == 0:
        print("Even")
    else:
        print("Odd")

if __name__ == '__main__':
    main()
