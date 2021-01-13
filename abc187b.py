def main():
    n = int(input())
    xy = [None for _ in range(n)]
    for i in range(n):
        xy[i] = tuple(map(int, input().split()))

    ans = 0
    for i in range(n):
        for j in range(i+1, n):
            if xy[i][0] == xy[j][0]:
                continue
            elif abs(xy[i][0] - xy[j][0]) >= abs(xy[i][1] - xy[j][1]):
                ans += 1

    print(ans)

if __name__ == '__main__':
    main()
