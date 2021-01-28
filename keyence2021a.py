def main():
    n = int(input())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    c = [0 for _ in range(n)]
    a_max = a[0]
    c[0] = b[0] * a_max
    for i in range(1, n):
        a_max = max(a_max, a[i])
        c[i] = max(c[i-1], b[i] * a_max)

    for x in c:
        print(x)

if __name__ == '__main__':
    main()
