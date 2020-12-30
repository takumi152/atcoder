from bisect import bisect

def main():
    n, m = map(int, input().split())
    h = list(map(int, input().split()))
    w = list(map(int, input().split()))

    h.sort()

    even_sum = [0 for _ in range(n//2+1)]
    for i in range(n//2):
        even_sum[i+1] = even_sum[i] + abs(h[i*2] - h[i*2+1])
    odd_sum = [0 for _ in range(n//2+1)]
    for i in range(n//2):
        odd_sum[i+1] = odd_sum[i] + abs(h[i*2+1] - h[i*2+2])

    ans = 10 ** 18
    for x in w:
        p = bisect(h, x)
        p1 = max(0, (p // 2) * 2)
        p2 = min(n-1, (p // 2) * 2 + 2)
        for y in (p1, p2):
            d = 0
            if y % 2 == 0:
                d = abs(x - h[y]) + even_sum[y//2] + odd_sum[n//2] - odd_sum[y//2]
            if d < ans:
                ans = d

    print(ans)

if __name__ == '__main__':
    main()
