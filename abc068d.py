
def main():
    buf = input()
    k = int(buf)

    n = 50
    a = []
    for i in range(n):
        a.append(n-i-1)
    p = k // 50
    for i in range(n):
        a[i] += p
    k %= 50
    i = 0
    while k > 0:
        a[i] += 1
        k -= 1
        i += 1
    print(n)
    print(' '.join([str(x) for x in a]))

if __name__ == '__main__':
    main()
