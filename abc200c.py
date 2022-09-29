def main():
    n = int(input())
    a = list(map(int, input().split()))

    count = [0 for _ in range(200)]
    for x in a:
        count[x % 200] += 1

    ans = 0
    for i in range(200):
        ans += (count[i] * (count[i] - 1)) // 2

    print(ans)

if __name__ == '__main__':
    main()
