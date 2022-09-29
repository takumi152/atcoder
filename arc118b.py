def main():
    k, n, m = map(int, input().split())
    a = list(map(int, input().split()))

    # find max(abs(error))
    ng = -1
    ok = n * m
    while abs(ng - ok) > 1:
        center = (ng + ok) // 2

        good = True
        lower_sum = 0
        upper_sum = 0
        for i in range(k):
            # 1 7
            # 20 140
            # 10 140 -> 2 20 (14 140)
            # 30 140 -> 4 20 (28 140)
            lower_bound = max(0, ((a[i] * m - center) + (n - 1)) // n)
            upper_bound = min(m, (a[i] * m + center) // n)
            if lower_bound > upper_bound:
                good = False
                break
            lower_sum += lower_bound
            upper_sum += upper_bound

        if good and lower_sum <= m and m <= upper_sum:
            ok = center
        else:
            ng = center

    lower_bound_list = [0 for _ in range(k)]
    upper_bound_list = [0 for _ in range(k)]
    for i in range(k):
        lower_bound_list[i] = max(0, ((a[i] * m - ok) + (n - 1)) // n)
        upper_bound_list[i] = min(m, (a[i] * m + ok) // n)

    ans = [0 for _ in range(k)]
    ans_sum = 0
    for i in range(k):
        ans[i] = lower_bound_list[i]
        ans_sum += lower_bound_list[i]

    for i in range(k):
        if ans_sum == m:
            break
        elif ans_sum + (upper_bound_list[i] - lower_bound_list[i]) <= m:
            ans[i] = upper_bound_list[i]
            ans_sum += upper_bound_list[i] - lower_bound_list[i]
        else:
            ans[i] += m - ans_sum
            ans_sum = m

    print(' '.join([str(x) for x in ans]))

if __name__ == '__main__':
    main()
