def main():
    a, b, n = map(int, input().split())

    x = (n - (b - 2) + (b - 1)) // b
    y = max(0, b * x - 1)
    ans = max((a * y) // b - a * (y // b), (a * n) // b - a * (n // b))

    print(ans)

if __name__ == '__main__':
    main()
