def main():
    a, b, c, x, y = map(int, input().split())

    ans = 1000000007
    for i in range(100001):
        cost = c * i * 2 + a * max(0, x - i) + b * max(0, y - i)
        if cost < ans:
            ans = cost

    print(ans)

if __name__ == '__main__':
    main()
