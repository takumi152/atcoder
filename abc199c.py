def main():
    n = int(input())
    s = list(input())
    q = int(input())
    t = [0 for _ in range(q)]
    a = [0 for _ in range(q)]
    b = [0 for _ in range(q)]
    for i in range(q):
        t[i], a[i], b[i] = map(int, input().split())

    flip = False
    for i in range(q):
        if t[i] == 1:
            if not flip:
                s[a[i]-1], s[b[i]-1] = s[b[i]-1], s[a[i]-1]
            else:
                s[(a[i]-1+n)%(2*n)], s[(b[i]-1+n)%(2*n)] = s[(b[i]-1+n)%(2*n)], s[(a[i]-1+n)%(2*n)]
        else:
            flip = not flip

    if flip:
        s = s[n:2*n] + s[0:n]

    print(''.join(s))


if __name__ == '__main__':
    main()
