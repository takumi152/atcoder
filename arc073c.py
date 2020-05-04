def main():
    n, t = map(int, input().split())
    a = list(map(int, input().split()))

    ans = 0
    for i in range(n-1):
        ans += min(t, a[i+1] - a[i])
    ans += t

    print(ans)

if __name__ == '__main__':
    main()
