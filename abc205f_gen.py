import random

def main():
    h = 100
    w = 100
    n = 100

    a = [None for _ in range(n)]
    b = [None for _ in range(n)]
    c = [None for _ in range(n)]
    d = [None for _ in range(n)]

    for i in range(n):
        a[i], b[i], c[i], d[i] = 1, 1, i+1, i+1
        if a[i] > c[i]:
            a[i], c[i] = c[i], a[i]
        if b[i] > d[i]:
            b[i], d[i] = d[i], b[i]

    print(h, w, n)
    for i in range(n):
        print(a[i], b[i], c[i], d[i])

if __name__ == '__main__':
    main()
