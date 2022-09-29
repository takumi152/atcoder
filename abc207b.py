def main():
    a, b, c, d = map(int, input().split())

    ng = 0
    ok = 10 ** 30
    while abs(ok - ng) > 1:
        center = (ng + ok) // 2
        if (a + b * center) <= c * center * d:
            ok = center
        else:
            ng = center

    if ok == 10 ** 30:
        print(-1)
    else:
        print(ok)

if __name__ == '__main__':
    main()
