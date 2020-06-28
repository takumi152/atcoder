import bisect

def main():
    n, m, k = map(int, input().split())
    a = list(map(int, input().split()))
    b = list(map(int, input().split()))

    a_culm = [0]
    for i in range(n):
        a_culm.append(a_culm[-1] + a[i])
    b_culm = [0]
    for i in range(m):
        b_culm.append(b_culm[-1] + b[i])

    ans = 0
    for i in range(n+1):
        rem = k - a_culm[i]
        if rem < 0:
            break
        b_count = bisect.bisect(b_culm, rem) - 1
        if i + b_count > ans:
            ans = i + b_count

    print(ans)

if __name__ == '__main__':
    main()
