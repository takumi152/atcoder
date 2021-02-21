def main():
    n, k = map(int, input().split())

    for i in range(k):
        g1 = sorted(list(str(n)), reverse = True)
        g2 = sorted(list(str(n)))
        n = int(''.join(g1)) - int(''.join(g2))

    print(n)

if __name__ == '__main__':
    main()
