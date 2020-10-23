def main():
    a, b, c, d = map(int, input().split())

    if a <= d and c <= b:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
