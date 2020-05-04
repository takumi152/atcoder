def main():
    n, m = map(int, input().split())

    ans = [None for _ in range(m)]
    used = set()
    a = 1
    b = n
    good = True
    for i in range(m):
        if b - a == n - (b - a):
            a += 1
            b -= 1
        ans[i] = (a, b)
        if (b - a) in used:
            good = False
            break
        else:
            used.add(b - a)
        if (n - (b - a)) in used:
            good = False
            break
        else:
            used.add(n - (b - a))
        a += 1
        b -= 1

    if not good:
        ans = [None for _ in range(m)]
        used = set()
        a = 1
        b = n-1
        good = True
        for i in range(m):
            if b - a == n - (b - a):
                a += 1
                b -= 1
            ans[i] = (a, b)
            if (b - a) in used:
                good = False
                break
            else:
                used.add(b - a)
            if (n - (b - a)) in used:
                good = False
                break
            else:
                used.add(n - (b - a))
            a += 1
            b -= 1

    if not good:
        ans = [None for _ in range(m)]
        used = set()
        a = 1
        b = n
        good = True
        dec = False
        for i in range(m):
            if b - a == n - (b - a):
                b -= 1
            if not dec and ((b - a) in used or (n - (b - a)) in used):
                b -= 1
            ans[i] = (a, b)
            if (b - a) in used:
                good = False
                break
            else:
                used.add(b - a)
            if (n - (b - a)) in used:
                good = False
                break
            else:
                used.add(n - (b - a))
            a += 1
            b -= 1


    for i in range(m):
        print(ans[i][0], ans[i][1])

if __name__ == '__main__':
    main()
