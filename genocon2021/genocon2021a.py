def main():
    m = int(input())
    a = [None for _ in range(m)]
    for i in range(m):
        a[i] = list(input())

    for i in range(m):
        a[i].reverse()
        for j in range(len(a[i])):
            if a[i][j] == 'A':
                a[i][j] = 'T'
            elif a[i][j] == 'T':
                a[i][j] = 'A'
            elif a[i][j] == 'G':
                a[i][j] = 'C'
            elif a[i][j] == 'C':
                a[i][j] = 'G'

    for x in a:
        print(''.join(x))

if __name__ == '__main__':
    main()
