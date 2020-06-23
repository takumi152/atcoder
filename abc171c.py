def main():
    n = int(input())

    t = 1

    ans = ''
    while n > 0:
        p = ((n - 1) // t) % 26
        ans += chr(ord('a') + p)
        n -= t * (p+1)
        t *= 26

    ans = ans[::-1]

    print(ans)


if __name__ == '__main__':
    main()
