def main():
    k = int(input())

    n = 50
    a = [None for _ in range(n)]
    for i in range(n):
        a[i] = i + (k + i) // n

    print(n)
    print(' '.join([str(x) for x in a]))

if __name__ == '__main__':
    main()
