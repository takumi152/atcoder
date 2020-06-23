def main():
    n, k = map(int, input().split())
    a = list(map(int, input().split()))

    cum_all = [0 for _ in range(n+1)]
    cum_positive = [0 for _ in range(n+1)]
    for i in range(n):
        cum_all[i+1] = cum_all[i] + a[i]
        cum_positive[i+1] = cum_positive[i] + max(0, a[i])

    ans = 0
    for i in range(n-k+1):
        score1 = (cum_positive[i] - cum_positive[0]) + (cum_positive[n] - cum_positive[i+k])
        if score1 > ans:
            ans = score1
        score2 = (cum_positive[i] - cum_positive[0]) + (cum_all[i+k] - cum_all[i]) + (cum_positive[n] - cum_positive[i+k])
        if score2 > ans:
            ans = score2

    print(ans)

if __name__ == '__main__':
    main()
