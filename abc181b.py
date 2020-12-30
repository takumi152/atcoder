def main():
    n = int(input())
    ab = [None for _ in range(n)]
    for i in range(n):
        ab[i] = list(map(int, input().split()))

    ans = 0
    for i in range(n):
        ans += (ab[i][1] * (ab[i][1] + 1) // 2) - (ab[i][0] * (ab[i][0] - 1) // 2)

    print(ans)

if __name__ == '__main__':
    main()
