def main():
    n = int(input())
    a = list(map(int, input().split()))

    a.sort()

    ans = 1
    for x in a:
        ans *= x
        if ans > 10 ** 18:
            print(-1)
            return

    print(ans)

if __name__ == '__main__':
    main()
