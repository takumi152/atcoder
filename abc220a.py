def main():
    a, b, c = map(int, input().split())

    for i in range(c, b+1, c):
        if a <= i:
            print(i)
            return

    print(-1)

if __name__ == '__main__':
    main()
