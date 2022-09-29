def main():
    n = int(input())
    lr = [tuple(map(int, input().split())) for _ in range(n)]

    ans = [0 for _ in range(n)]
    for i, x in enumerate(lr):
        delta = max(0, x[1] - x[0] * 2 + 1)
        ans[i] = (delta * (delta + 1)) // 2

    for x in ans:
        print(x)

if __name__ == '__main__':
    main()
