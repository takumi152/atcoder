def main():
    x, y, a, b = map(int, input().split())

    ans = 0
    p = x
    c = 0
    while p < y:
        e = c + (y - p - 1) // b
        if e > ans:
            ans = e
        p *= a
        c += 1

    print(ans)

if __name__ == '__main__':
    main()
