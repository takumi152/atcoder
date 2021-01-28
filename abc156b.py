def main():
    n, k = map(int, input().split())

    ans = 0
    while n > 0:
        n //= k
        ans += 1

    print(ans)

if __name__ == '__main__':
    main()
