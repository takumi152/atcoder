def main():
    n = int(input())
    x = list(map(int, input().split()))

    m = 0
    for i in range(n):
        m += abs(x[i])

    e = 0.0
    for i in range(n):
        e += x[i] * x[i]
    e = e ** 0.5

    c = 0
    for i in range(n):
        c = max(c, abs(x[i]))

    print(m)
    print(e)
    print(c)

if __name__ == '__main__':
    main()
