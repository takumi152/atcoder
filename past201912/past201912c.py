
def main():
    buf = input()
    buflist = buf.split()
    abcdef = list(map(int, buflist))

    abcdef.sort()
    print(abcdef[3])

if __name__ == '__main__':
    main()
