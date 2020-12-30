def main():
    n = int(input())

    ans = 0
    for i in range(1, n+1):
        good = True
        x = i
        while x > 0:
            if x % 10 == 7:
                good = False
                break
            x //= 10
        x = i
        while x > 0:
            if x % 8 == 7:
                good = False
                break
            x //= 8
        if good: ans += 1

    print(ans)

if __name__ == '__main__':
    main()
