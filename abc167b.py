def main():
    a, b, c, k = map(int, input().split())

    total = 0
    rem = k
    total += min(a, rem)
    rem = max(0, rem - a)
    rem = max(0, rem - b)
    total -= min(c, rem)
    rem = max(0, rem - c)

    print(total)

if __name__ == '__main__':
    main()
