def main():
    n = int(input())

    a = 1
    b = 1
    while True:
        while True:
            if 3 ** a + 5 ** b == n:
                print(a, b)
                return
            elif 3 ** a + 5 ** b < n:
                b += 1
            else:
                b = 1
                break
        a += 1
        if 3 ** a + 5 ** b > n:
            print(-1)
            return

if __name__ == '__main__':
    main()
