def main():
    n, m, l = map(int, input().split())
    p, q, r = map(int, input().split())

    ans = 0

    num = 1
    num *= n // p
    num *= m // q
    num *= l // r
    ans = max(ans, num)

    num = 1
    num *= n // p
    num *= l // q
    num *= m // r
    ans = max(ans, num)

    num = 1
    num *= m // p
    num *= n // q
    num *= l // r
    ans = max(ans, num)

    num = 1
    num *= m // p
    num *= l // q
    num *= n // r
    ans = max(ans, num)

    num = 1
    num *= l // p
    num *= n // q
    num *= m // r
    ans = max(ans, num)

    num = 1
    num *= l // p
    num *= m // q
    num *= n // r
    ans = max(ans, num)

    print(ans)

if __name__ == '__main__':
    main()
