def main():
    n, m = map(int, input().split())
    a = list(map(int, input().split()))

    t = 0
    for x in a:
        t += x

    ans = n - t
    if ans >= 0:
        print(ans)
    else:
        print(-1)

if __name__ == '__main__':
    main()
