def main():
    a, b = map(int, input().split())

    maximum = 2 * a + 100

    print(maximum - b)

if __name__ == '__main__':
    main()
