def main():
    x, y = map(int, input().split())

    if abs(y - x) < 3:
        print('Yes')
    else:
        print('No')

if __name__ == '__main__':
    main()
