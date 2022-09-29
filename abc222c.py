def rps(x, y):
    if x == y:
        return 0
    elif (x == 'G' and y == 'C') or (x == 'C' and y == 'P') or (x == 'P' and y == 'G'):
        return 1
    else:
        return -1

def main():
    n, m = map(int, input().split())
    a = [None for _ in range(2 * n)]
    for i in range(2 * n):
        a[i] = input()

    win_count = [0 for _ in range(2 * n)]
    leaderboard = [i for i in range(2 * n)]
    for i in range(m):
        for j in range(n):
            res = rps(a[leaderboard[2 * j]][i], a[leaderboard[2 * j + 1]][i])
            if res == 1:
                win_count[leaderboard[2 * j]] += 1
            elif res == -1:
                win_count[leaderboard[2 * j + 1]] += 1
        leaderboard.sort(key = lambda x: (-win_count[x], x))

    for x in leaderboard:
        print(x + 1)

if __name__ == '__main__':
    main()
