def main():
    n, m = map(int, input().split())
    explosive = [[[False for _ in range(n)] for _ in range(n)] for _ in range(n)]
    for i in range(m):
        a, b, c = map(int, input().split())
        explosive[a-1][b-1][c-1] = True

    ans = 0
    for mix in range(2 ** n):
        trigger = [False for _ in range(n)]
        good = True
        for i in range(n):
            for j in range(i+1, n):
                for k in range(j+1, n):
                    if not explosive[i][j][k]:
                        continue
                    elif (mix & (1 << i)) and (mix & (1 << j)) and (mix & (1 << k)):
                        good = False
                        break
                    elif (mix & (1 << i)) and (mix & (1 << j)):
                        trigger[k] = True
                    elif (mix & (1 << i)) and (mix & (1 << k)):
                        trigger[j] = True
                    elif (mix & (1 << j)) and (mix & (1 << k)):
                        trigger[i] = True
        if good and sum(trigger) > ans:
            ans = sum(trigger)

    print(ans)

if __name__ == '__main__':
    main()
