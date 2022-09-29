def main():
    n, p = map(int, input().split())

    print(((p - 1) * pow(p - 2, n - 1, int(1e9) + 7)) % (int(1e9) + 7))

if __name__ == '__main__':
    main()
