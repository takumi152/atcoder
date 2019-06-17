
def main():
    buf = input()
    N = int(buf)
    buf = input()
    buflist = buf.split()
    A = list(map(int, buflist))
    count = 0
    while all(list(map(lambda a: a % 2 == 0, A))):
        A = list(map(lambda a: a / 2, A))
        count += 1
    print(count)

if __name__ == '__main__':
    main()
