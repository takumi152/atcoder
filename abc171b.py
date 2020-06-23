def main():
    n, k = map(int, input().split())
    p = list(map(int, input().split()))

    p.sort()

    ans = 0
    for i in range(k):
        ans += p[i]

    print(ans)

if __name__ == '__main__':
    main()
