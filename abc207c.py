def main():
    n = int(input())
    tlr = [None for _ in range(n)]
    for i in range(n):
        tlr[i] = list(map(int, input().split()))

    for i in range(n):
        tlr[i][1] *= 2
        tlr[i][2] *= 2
        if tlr[i][0] in (3, 4):
            tlr[i][1] += 1
        if tlr[i][0] in (2, 4):
            tlr[i][2] -= 1

    ans = 0
    for i in range(n):
        for j in range(i+1, n):
            if tlr[j][1] <= tlr[i][2] and tlr[i][1] <= tlr[j][2]:
                ans += 1

    print(ans)

if __name__ == '__main__':
    main()
