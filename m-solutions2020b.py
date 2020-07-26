def main():
    a, b, c = map(int, input().split())
    k = int(input())

    op = 0
    while not (a < b and b < c):
        if not a < b:
            b *= 2
            op += 1
        elif not b < c:
            c *= 2
            op += 1

    if op <= k:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
