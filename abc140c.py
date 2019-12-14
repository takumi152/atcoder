
def main():
    buf = input()
    n = int(buf)
    buf = input()
    buflist = buf.split()
    b = list(map(int, buflist))

    a = []
    for i in range(n):
        a.append(100001)
    for i in range(n-1):
        a[i] = min(a[i], b[i])
        a[i+1] = min(a[i+1], b[i])

    ans = sum(a)
    print(ans)

if __name__ == '__main__':
    main()
