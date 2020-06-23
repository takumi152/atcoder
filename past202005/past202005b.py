def main():
    n, m, q = map(int, input().split())
    s = [None for _ in range(q)]
    for i in range(q):
        s[i] = list(map(int, input().split()))

    score = [n for _ in range(m)]
    solved = [[False for _ in range(m)] for _ in range(n)]
    for i in range(q):
        if s[i][0] == 1:
            ans = 0
            for j in range(m):
                if solved[s[i][1]-1][j]:
                    ans += score[j]
            print(ans)
        else:
            solved[s[i][1]-1][s[i][2]-1] = True
            score[s[i][2]-1] -= 1

if __name__ == '__main__':
    main()
