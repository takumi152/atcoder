def main():
    n = int(input())
    a = list(map(int, input().split()))

    ans = 0
    now = 0
    culm = 0
    culm_max = 0
    for i in range(n):
        culm += a[i]
        if culm > culm_max:
            culm_max = culm
        if now + culm_max > ans:
            ans = now + culm_max
        now += culm

    print(ans)

if __name__ == '__main__':
    main()
