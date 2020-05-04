def main():
    n, a, b = map(int, input().split())

    if n == 1:
        if a != b:
            print(0)
        else:
            print(1)
        return

    if a > b:
        print(0)
        return

    print((b - a) * (n - 2) + 1)

if __name__ == '__main__':
    main()
