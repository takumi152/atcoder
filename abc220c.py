def main():
    n = int(input())
    a = list(map(int, input().split()))
    x = int(input())

    ans = (x // sum(a)) * n
    rem = x % sum(a)
    for i in range(n):
        ans += 1
        rem -= a[i]
        if rem < 0:
            break

    print(ans)

if __name__ == '__main__':
    main()
