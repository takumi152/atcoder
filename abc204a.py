def main():
    x, y = map(int, input().split())

    if x == y:
        print(x)
    else:
        s = {x, y}
        if 0 not in s:
            print(0)
        elif 1 not in s:
            print(1)
        elif 2 not in s:
            print(2)

if __name__ == '__main__':
    main()
