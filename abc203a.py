def main():
    a, b, c = map(int, input().split())

    if a == b:
        print(c)
    elif a == c:
        print(b)
    elif b == c:
        print(a)
    else:
        print(0)

if __name__ == '__main__':
    main()
