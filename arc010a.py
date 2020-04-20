def main():
    n, m, a, b = map(int, input().split())
    c = [None] * m
    for i in range(m):
        c[i] = int(input())

    rem = n
    for i in range(m):
        if rem <= a:
            rem += b
        rem -= c[i]
        if rem < 0:
            print(i+1)
            return

    print('complete')

if __name__ == '__main__':
    main()
