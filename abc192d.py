def base_x(x, n, m):
    value = 0
    for i, v in enumerate(reversed(n)):
        value += int(v) * x ** i
        if value > m:
            return False
    return True

def main():
    n = input()
    m = int(input())

    if len(n) == 1:
        if int(n) <= m:
            print(1)
        else:
            print(0)
        return

    d = 0
    for x in n:
        d = max(d, int(x))

    ok = d
    ng = 10 ** 19
    while abs(ok - ng) > 1:
        center = (ok + ng) // 2
        if base_x(center, n, m):
            ok = center
        else:
            ng = center

    print(ok - d)

if __name__ == '__main__':
    main()
